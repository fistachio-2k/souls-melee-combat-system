// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equippable.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class VOICEPROJECT_API ABaseWeapon : public AActor, public IEquippable
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
	virtual void Tick(float DeltaTime) override;
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	virtual void AttachActor(FName SocketNameToAttach) override;
	virtual UPrimitiveComponent* GetItemMesh() override;
	bool IsWeaponInHande() const;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
	FName HeapSocketName;

	UPROPERTY(EditAnywhere)
	FName HandSocketName;

private:
	bool bIsHandEquipped = false;

};
