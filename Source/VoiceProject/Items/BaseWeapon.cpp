// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceProject/Characters/FremenCharacter.h"
#include "VoiceProject/Components/CollisionTraceComponent.h"
#include "VoiceProject/Components/CombatComponent.h"
#include "VoiceProject/Utils/Logger.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);

	CollisionTraceComponent = CreateDefaultSubobject<UCollisionTraceComponent>(TEXT("CollisionTrace"));
	AddOwnedComponent(CollisionTraceComponent);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionTraceComponent)
	{
		CollisionTraceComponent->OnHit.AddDynamic(this, &ABaseWeapon::WeaponHit);
	}
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Interact(AActor* Caller)
{
	if (const AFremenCharacter* Character = Cast<AFremenCharacter>(Caller))
	{
		// TODO: Consider make a getter for the CombatComponent to avoid casting
		UActorComponent* ActorComponent = Character->GetComponentByClass(UCombatComponent::StaticClass());
		if (UCombatComponent* CombatComponent = Cast<UCombatComponent>(ActorComponent))
		{
			CombatComponent->SetMainWeapon(this);
		}
	}
}

void ABaseWeapon::OnEquipped()
{
	IEquippable::OnEquipped();
	AttachActor(HeapSocketName);
	CollisionTraceComponent->SetCollisionMesh(GetItemMesh());
	CollisionTraceComponent->AddActorToIgnore(GetOwner());
}

void ABaseWeapon::OnUnequipped()
{
	IEquippable::OnUnequipped();
}

void ABaseWeapon::AttachActor(FName SocketNameToAttach)
{
	auto Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr)
	{
		Logger::Log(ELogLevel::WARNING, FString::Printf(TEXT("Can't attach %s, owner cast to character failed."), *this->GetName()));
		return;
	}
	bIsHandEquipped = SocketNameToAttach == HandSocketName;
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketNameToAttach);
}

UPrimitiveComponent* ABaseWeapon::GetItemMesh()
{
	return MeshComponent;
}

bool ABaseWeapon::IsWeaponInHand() const
{
	return bIsHandEquipped;
}

void ABaseWeapon::WeaponHit(FHitResult HitResult)
{
	Logger::Log(ELogLevel::INFO, __FUNCTION__);
	UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), Damage, HitResult.ImpactNormal, HitResult, GetInstigatorController(), this,  UDamageType::StaticClass());
}

