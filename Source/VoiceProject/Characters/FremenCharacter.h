// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combatable.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Utils/StateMachine.h"
#include "FremenCharacter.generated.h"

class UCombatComponent;
class URagdollComponent;
class ABaseWeapon;
class FChangeCharacterMaxSpeedAction;

enum ECharacterStates
{
	Idle,
	Attacking,
	Dodging,
	GeneralAction,
	Disabled,
	Dead
};

// TODO: move me to a blueprint visible class variable.
enum ECharacterMovementMode
{
	Walking = 500,
	Sprinting = 800
};

UCLASS()
class VOICEPROJECT_API AFremenCharacter : public ACharacter, public ICombatable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFremenCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual FRotator GetSignificantInputRotation(float Threshold) override;
	virtual void AttackContinue() override;
	virtual void ResetMovementState() override;
	virtual bool CanReceiveDamage() override;
	
	void InstallStateMachineHandlers();

private:
	void TrySpawnMainWeapon();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	void ToggleSprint();
	void ToggleWalk();
	void ToggleWeapon();
	void Interact();
	void Dodge();

	virtual void Attack() override;

	void PerformAttack(unsigned int AttackIndex, bool IsRandom = false);
	void PerformDodge();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void PerformDeath();
	void DestroyCharacter();
	
	UFUNCTION()
	void OnReceivePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UPROPERTY()
	UCombatComponent* CombatComponent;
	
	UPROPERTY()
	URagdollComponent* RagdollComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere)
	USoundBase* HitCue;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BloodEmitter;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation") // TODO: consider move montage to CombatComp or BaseWeapon or some other state machine
	UAnimMontage* DodgeMontage;

	UPROPERTY(VisibleAnywhere)
	float Health = 100.f;
	float RotationRate = 100.f;

	StateMachine<ECharacterStates> CharacterStateMachine;

	FChangeCharacterMaxSpeedAction* ChangeMovementSpeedLatentAction;
};
