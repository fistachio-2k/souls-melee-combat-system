// Fill out your copyright notice in the Description page of Project Settings.


#include "PickWeaponInteraction.h"

#include "BaseWeapon.h"


// Sets default values
APickWeaponInteraction::APickWeaponInteraction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APickWeaponInteraction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickWeaponInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickWeaponInteraction::Interact(AActor* Caller)
{
	// Spawn item and equip it.
	if (ItemClass != nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			
			SpawnParameters.Owner = Caller;
			SpawnParameters.Instigator =  Cast<APawn>(Caller);

			if (ABaseWeapon* Item = World->SpawnActor<ABaseWeapon>(ItemClass, GetActorTransform(), SpawnParameters))
			{
				Item->OnEquipped();
			}
		}
	}
}

