// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combatable.h"
#include "Focusable.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Items/BaseWeapon.h"
#include "Utils/StateMachine.h"
#include "FremenCharacter.generated.h"

class UWidgetComponent;
class UFocusComponent;
class UCombatComponent;
class URagdollComponent;
class ABaseWeapon;
class UMotionWarpingComponent;

enum ECharacterStates
{
	Idle,
	Attacking,
	Dodging,
	GeneralAction,
	Disabled,
	Dead
};

UCLASS()
class SOULSMELEECOMBATSYSTEM_API AFremenCharacter : public ACharacter, public ICombatable, public IFocusable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFremenCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual FRotator GetSignificantInputRotation(float Threshold) override;
	virtual void AttackContinue() override;
	virtual void ResetMovementState() override;
	virtual bool CanReceiveDamage() override;
	
	virtual bool CanBeFocused() override;
	virtual void OnFocused(bool bIsFocused) override;
	
	void InstallStateMachineHandlers();

	UPROPERTY(EditAnywhere, Category = "UI")
	UWidgetComponent* InFocusWidget;

private:
	void TrySpawnMainWeapon();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	
	void ToggleWeapon();
	void Interact();
	void Dodge();
	void Focus();

	virtual void Attack() override;
	void ClearChargeAttack();
	void HeavyAttack();
	void StartChargeAttack();

	void SetWeaponDamageType(EAttackType Attack) const;
	void PerformAttack(EAttackType AttackType, bool IsRandom = false);
	void ApplyMotionWarping(FName WarpTargetName) const;
	void PerformDodge();
	
	UFUNCTION(BlueprintCallable, Category = "Test")
	void PerformDeath();
	void DestroyCharacter();
	
	UFUNCTION()
	void OnReceivePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	void ApplyHitReaction(const FVector& HitPoint) const;
	void PlayHitReactMontage(const FName& Section) const;
	StateMachine<ECharacterStates> CharacterStateMachine;
	
	UPROPERTY()
	UCombatComponent* CombatComponent;
	
	UPROPERTY()
	URagdollComponent* RagdollComponent;

	UPROPERTY()
	UFocusComponent* FocusComponent;
	
	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitCue;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BloodEmitter;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DodgeMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation Warping")
	float MinWarpingDistance = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Animation Warping")
	float WarpingTargetOffsetFactor = 100.f;
	
	UPROPERTY(VisibleAnywhere)
	float Health = 100.f;

	UPROPERTY(EditAnywhere)
	float ChargeAttackDuration = 0.5f;
	
	float RotationRate = 100.f;
	bool bIsChargedAttackReady = false;
	FTimerHandle ChargeAttackTimerHandle;
};
