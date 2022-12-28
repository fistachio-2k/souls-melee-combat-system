// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equippable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquippable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOICEPROJECT_API IEquippable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void AttachActor(FName SocketName) = 0;
	virtual UPrimitiveComponent* GetItemMesh() = 0;
	
	virtual void SetIsEquipped(bool IsEquipped);
	virtual bool IsEquipped();
	
private:
	bool bIsEquipped = false;
};
