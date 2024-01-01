// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equippable.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USphereComponent;
class UCollisionTraceComponent;

enum EAttackType
{
	Light,
	Heavy,
	Charge,
	Sprint
};

UCLASS()
class SOULSMELEECOMBATSYSTEM_API ABaseWeapon : public AActor, public IEquippable, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Interact(AActor* Caller) override;
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	virtual void AttachActor(FName SocketNameToAttach) override;
	virtual UPrimitiveComponent* GetItemMesh() override;
	bool IsWeaponInHand() const;
	const TArray<UAnimMontage*>& GetAttackMontages(EAttackType AttackType) const;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionSphere;
	
	UPROPERTY(EditAnywhere)
	int Damage = 50;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DrawWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SheatheWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimMontage*> HeavyAttackMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimMontage*> ChargeAttackMontages;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	FName HeapSocketName;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FName HandSocketName;
	
	UPROPERTY()
	TSubclassOf<UDamageType> DamageType;

private:
	UFUNCTION()
	void WeaponHit(const FHitResult& HitResult);
	
	bool bIsHandEquipped = false;

	UPROPERTY(VisibleAnywhere)
	UCollisionTraceComponent* CollisionTraceComponent;
};
