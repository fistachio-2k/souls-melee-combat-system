// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceProject/Items/BaseWeapon.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOICEPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	ABaseWeapon* GetMainWeapon() const;
	void SetMainWeapon(ABaseWeapon* NewWeapon);
	bool IsCombatEnabled() const;
	void SetCombatEnabled(bool IsCombatEnabled);
	void ResetCombat();

private:
	ABaseWeapon* MainWeapon;
	bool bIsCombatEnabled;
	bool bIsAttacking;
	bool bIsAttackSaved;
	uint8 AttackCount;
	
	
};
