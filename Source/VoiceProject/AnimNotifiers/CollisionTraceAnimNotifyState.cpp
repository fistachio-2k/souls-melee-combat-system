// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTraceAnimNotifyState.h"

#include "VoiceProject/Components/CollisionTraceComponent.h"
#include "VoiceProject/Components/CombatComponent.h"

void UCollisionTraceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, FrameDeltaTime, EventReference);

	UActorComponent* CombatActorComponent = MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass());
	if (const UCombatComponent* CombatComponent = Cast<UCombatComponent>(CombatActorComponent))
	{
		UActorComponent* CollisionActorComponent = CombatComponent->GetMainWeapon()->GetComponentByClass(UCollisionTraceComponent::StaticClass());
		if (UCollisionTraceComponent* TraceComponent = Cast<UCollisionTraceComponent>(CollisionActorComponent))
		{
			TraceComponent->EnableCollision();
		}
	}
}

void UCollisionTraceAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	UActorComponent* CombatActorComponent = MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass());
	if (const UCombatComponent* CombatComponent = Cast<UCombatComponent>(CombatActorComponent))
	{
		UActorComponent* CollisionActorComponent = CombatComponent->GetMainWeapon()->GetComponentByClass(UCollisionTraceComponent::StaticClass());
		if (UCollisionTraceComponent* TraceComponent = Cast<UCollisionTraceComponent>(CollisionActorComponent))
		{
			TraceComponent->DisableCollision();
		}
	}
}

