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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleFocus();

private:
	void Focus();
	bool FindTarget(IFocusable**);
	void SetRotationMode(ERelativeOrientation OrientTo) const;
	void UpdateOwnerRotationMode();
	bool bIsInFocus;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DebugTrace;
	
	UPROPERTY(EditAnywhere)
	float FocusDistance = 500.f;
	
	IFocusable* ActorInFocus;
	
	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	AController* OwnerController;
	UPROPERTY()
	UCameraComponent* FollowCamera;
};
