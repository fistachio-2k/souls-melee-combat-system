// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RagdollComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEPROJECT_API URagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URagdollComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Init();

	void EnableRagdoll() const;

private:
	UPROPERTY()
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	UPROPERTY(EditAnywhere)
	FName PelvisBoneName = "Pelvis";
};
