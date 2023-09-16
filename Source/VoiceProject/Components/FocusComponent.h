// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FocusComponent.generated.h"

class IFocusable;
class UActorComponent;
class UCameraComponent;

enum ERelativeOrientation
{
	OrientToMovement,
	OrientToCamera
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICEPROJECT_API UFocusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFocusComponent();
	
	void ToggleFocus();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Focus();
	void ChangeRotation() const;
	void UpdateFocus();
	bool FindTarget(IFocusable**);
	void SetRotationMode(ERelativeOrientation OrientTo) const;
	void UpdateOwnerRotationMode() const;
	bool bIsInFocus;
	bool bIsEndOfFocusTransition;
	
	IFocusable* FocusTarget;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DebugTrace;
	UPROPERTY(EditAnywhere)
	float FocusDistance = 500.f;
	UPROPERTY(EditAnywhere)
	float Speed = 9;
	FTimerHandle EndOfFocusTimerHandle;
	
	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	AController* OwnerController;
	UPROPERTY()
	UCameraComponent* FollowCamera;
	UPROPERTY()
	AActor* ActorInFocus;
};
