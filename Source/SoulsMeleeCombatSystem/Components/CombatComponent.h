﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoulsMeleeCombatSystem/Items/BaseWeapon.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULSMELEECOMBATSYSTEM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	ABaseWeapon* GetMainWeapon() const;
	void SetMainWeapon(ABaseWeapon* NewWeapon);
	void SetCombatEnabled(bool IsCombatEnabled);
	void ResetCombat();
	
	bool IsCombatEnabled() const;
	bool CanAttack() const;
	
	bool bIsAttacking;
	bool bIsAttackSaved;
	uint8 AttackCount;
	
private:
	UPROPERTY()
	ABaseWeapon* MainWeapon;
	
	bool bIsCombatEnabled;
};
