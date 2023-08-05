// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FocusComponent.h"

#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Focusable.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	
	UpdateFocus();
}

void UFocusComponent::ToggleFocus()
{
	SetComponentTickEnabled(!bIsInFocus);
	
	if (bIsInFocus)
	{
		bIsInFocus = false;
		SetRotationMode(OrientToCamera);
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

void UFocusComponent::ChangeRotation()
{
	constexpr int Threshold = 100;
	const FVector SourceLocation = GetOwner()->GetActorLocation();
	const FVector TargetLocation = Cast<AActor>(ActorInFocus)->GetActorLocation() - FVector(0, 0, Threshold);
			
	// Calculate the rotation from the source location to the target location
	const FRotator LockAtRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, TargetLocation);

	// Interpolate rotation according to speed
	const auto InterpolatedRotation = UKismetMathLibrary::RInterpTo(OwnerController->GetControlRotation(), LockAtRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), Speed) ;
			
	// Set rotation values
	const float Pitch = InterpolatedRotation.Pitch;
	const float Yaw = InterpolatedRotation.Yaw;
	const float Roll = OwnerController->GetControlRotation().Roll;;

	// Set owner rotation
	OwnerController->SetControlRotation(FRotator(Pitch, Yaw, Roll));
}

void UFocusComponent::UpdateFocus()
{
	if (OwnerCharacter && OwnerController)
	{
		if (ActorInFocus->CanBeFocused())
		{
			ChangeRotation();
			return;
		}
	}
	
	ToggleFocus(); // Toggle focus off
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

