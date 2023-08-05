// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FocusComponent.h"

#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Focusable.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/Logger.h"

// Sets default values for this component's properties
UFocusComponent::UFocusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFocusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter != nullptr)
	{
		OwnerController = OwnerCharacter->GetController();
		if (UActorComponent* Actor = OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()))
		{
			FollowCamera = Cast<UCameraComponent>(Actor);
		}
	}
	else
	{
		Logger::Log(ELogLevel::ERROR, "Casting component owner to ACharacter has failed!");
	}
}


// Called every frame
void UFocusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsInFocus)
	{
		return;
	}
	
	// ...
}

void UFocusComponent::ToggleFocus()
{
	SetComponentTickEnabled(!bIsInFocus);
	
	if (bIsInFocus)
	{
		bIsInFocus = false;
	}
	else
	{
		Focus();
	}
}


void UFocusComponent::Focus()
{
	IFocusable* OutFocusable;
	if (FindTarget(&OutFocusable) && OutFocusable->CanBeFocused())
	{
		ActorInFocus = OutFocusable;
		UpdateOwnerRotationMode();
		bIsInFocus = true;
	}
}

bool UFocusComponent::FindTarget(IFocusable** OutFocusable)
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + FollowCamera->GetForwardVector() * FocusDistance;
	float Radius = 100.0f; 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_Pawn)};
	TArray<AActor*> ActorsToIgnore = {GetOwner()};
	FHitResult OutHit; 

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, Radius, ObjectTypes,
		false, ActorsToIgnore, DebugTrace,OutHit,true)
		&& OutHit.GetActor() != nullptr)
	{
		if (IFocusable* Focusable = Cast<IFocusable>(OutHit.GetActor()))
		{
			*OutFocusable = Focusable;
			return true;
		}
	}

	// Sphere trace didn't hit anything that implements IFocusable
	ActorInFocus = nullptr;
	return false;
}

void UFocusComponent::SetRotationMode(ERelativeOrientation OrientTo) const
{
	switch (OrientTo)
	{
		case OrientToCamera:
			// TODO: remove bUseControllerRotationYaw set after check for mistake
			OwnerCharacter->bUseControllerRotationYaw = false;
			OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
			break;
		
		case OrientToMovement:
			OwnerCharacter->bUseControllerRotationYaw = false;
			OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			break;
	}
}

void UFocusComponent::UpdateOwnerRotationMode()
{
	const auto CombatComponent = Cast<UCombatComponent>(OwnerCharacter->GetComponentByClass(UCombatComponent::StaticClass()));
	if (CombatComponent && CombatComponent->IsCombatEnabled() && bIsInFocus)
	{
		SetRotationMode(OrientToCamera);
	}
	else
	{
		SetRotationMode(OrientToMovement);
	}
}

