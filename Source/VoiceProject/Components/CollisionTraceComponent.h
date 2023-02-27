// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionTraceComponent.generated.h"

DECLARE_DELEGATE_OneParam(FHitResultDelegate, FHitResult)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOICEPROJECT_API UCollisionTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCollisionTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EnableCollision();
	void DisableCollision();
	bool CollisionTrace();
	void ClearHitActors();

	UPROPERTY(EditAnywhere, Category="Initialization")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, Category="Initialization")
	FName EndSocketName;

	UPROPERTY(EditAnywhere, Category="Initialization")
	float TraceRadius;

	UPROPERTY(EditAnywhere, Category="Initialization")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectType;

	UPROPERTY(EditAnywhere, Category="Initialization")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category="Initialization")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;
	
private:
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY()
	UPrimitiveComponent* MeshComponent;

	FHitResultDelegate OnHit;
};
