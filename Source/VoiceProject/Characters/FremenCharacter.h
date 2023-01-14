// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FremenCharacter.generated.h"

class ABaseWeapon;

UCLASS()
class VOICEPROJECT_API AFremenCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFremenCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	
	void ToggleWeapon();
	void Interact();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float RotationRate = 100.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DrawWeaponMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* SheatheWeaponMontage;

	ABaseWeapon& GetMainWeapon() const;

	void SetMainWeapon(ABaseWeapon* Weapon);
	
private:
	bool bIsCombatEnabled;
	ABaseWeapon* MainWeapon;

	void SetCombatEnabled(bool IsCombatEnabled);
};
