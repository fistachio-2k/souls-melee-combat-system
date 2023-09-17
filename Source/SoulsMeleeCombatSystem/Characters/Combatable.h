// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combatable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULSMELEECOMBATSYSTEM_API ICombatable 
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attack() = 0;
	virtual void AttackContinue() = 0;
	virtual void ResetMovementState() = 0;
	virtual FRotator GetSignificantInputRotation(float Threshold) = 0;
	virtual bool CanReceiveDamage() = 0;
};
