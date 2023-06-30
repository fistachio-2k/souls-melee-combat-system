// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FocusComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
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
	if (bIsInFocus)
	{
		bIsInFocus = false;
	}
	else
	{
		Focus();
	}
	
	SetComponentTickEnabled(!bIsInFocus);
}


void UFocusComponent::Focus()
{
	if (FindTarget())
	{
		
	}
}

bool UFocusComponent::FindTarget()
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + FollowCamera->GetForwardVector() * FocusDistance;
	float Radius = 100.0f; 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); 
	bool bTraceComplex = false; 
	TArray<AActor*> ActorsToIgnore = {GetOwner()};
	EDrawDebugTrace::Type DrawDebugType = DebugTrace; 
	FHitResult OutHit; 
	bool bIgnoreSelf = true;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		Radius,
		ObjectTypes,
		bTraceComplex,
		ActorsToIgnore,
		DrawDebugType,
		OutHit,
		bIgnoreSelf))
	{
		// Sphere trace hit something
		ActorInFocus = OutHit.GetActor();
		FVector HitLocation = OutHit.ImpactPoint;
		return true;
	}

	// Sphere trace didn't hit anything
	ActorInFocus = nullptr;
	return false;
}

