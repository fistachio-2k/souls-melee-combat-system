// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "HeavyDamage.generated.h"

/**
 * 
 */
UCLASS()
class SOULSMELEECOMBATSYSTEM_API UHeavyDamage : public UDamageType
{
	GENERATED_BODY()
	uint32 bCausedByWorld = 0;
};
