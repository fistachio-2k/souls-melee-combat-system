// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FremenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOICEPROJECT_API UFremenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void UpdateIsCombatEnabled(bool IsCombatEnabled);

	UFUNCTION(BlueprintGetter, meta = (BlueprintThreadSafe))
	bool IsCombatEnabled() const { return bIsCombatEnabled; }

private:
	UPROPERTY(BlueprintGetter=IsCombatEnabled)
	bool bIsCombatEnabled;
};
