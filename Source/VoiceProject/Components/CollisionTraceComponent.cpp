// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTraceComponent.h"

#include "Kismet/GameplayStatics.h"
#include "VoiceProject/Items/BaseWeapon.h"
#include "VoiceProject/Utils/Logger.h"


// Sets default values for this component's properties
UCollisionTraceComponent::UCollisionTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCollisionTraceComponent::BeginPlay()
{ 
	Super::BeginPlay();
	SetComponentTickEnabled(false);

	// Get Mesh from owner
	if (UObject* Object = GetOwner()->GetDefaultSubobjectByName(MeshName))
	{
		if (UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Object))
		{
			MeshComponent = Component;
		}
		else
		{
			Logger::Log(ELogLevel::ERROR, "UCollisionTraceComponent can't find Mesh component.");
		}
	}

	if (bIgnorePlayerPawn)
	{
		ActorsToIgnore.AddUnique(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	}
}


// Called every frame
void UCollisionTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CollisionTrace();
	// ...
}

void UCollisionTraceComponent::EnableCollision()
{
	ClearHitActors();
	SetComponentTickEnabled(true);
}

void UCollisionTraceComponent::DisableCollision()
{
	SetComponentTickEnabled(false);
}

void UCollisionTraceComponent::CollisionTrace()
{
	if (MeshComponent == nullptr)
	{
		return;
	}

	const FVector Start = MeshComponent->GetSocketLocation(StartSocketName);
	const FVector End = MeshComponent->GetSocketLocation(EndSocketName);

	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius, CollisionObjectType, false, ActorsToIgnore, DebugType.GetValue(), OutHits, true);

	if(!OutHits.IsEmpty())
	{
		for (FHitResult HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!AlreadyHitActors.Contains(HitActor) && OnHit.IsBound())
			{
				AlreadyHitActors.Emplace(HitActor);
				OnHit.Broadcast(HitResult);
			}
		}
	}
	
}

void UCollisionTraceComponent::ClearHitActors()
{
	AlreadyHitActors.Empty();
}