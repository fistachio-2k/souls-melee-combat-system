// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::OnEquipped()
{
	// IEquippable::Execute_OnEquipped(this);
	AttachActor(HeapSocketName);
}

void ABaseWeapon::OnUnequipped()
{
	// Super::OnUnequipped();
}

void ABaseWeapon::AttachActor(FName SocketNameToAttach)
{
	auto Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp,
			Warning,
			TEXT("Can't attach %s, owner cast to character failed."),
			*this->GetName());
		
		return;
	}
	bIsHandEquipped = SocketNameToAttach == HandSocketName;
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNameToAttach);
}

UPrimitiveComponent* ABaseWeapon::GetItemMesh()
{
	return MeshComponent;
}

bool ABaseWeapon::IsWeaponInHande() const
{
	return bIsHandEquipped;
}

