// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetAttackAnimNotify.h"
#include "VoiceProject/Characters/FremenCharacter.h"

void UResetAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AFremenCharacter* Fremen = Cast<AFremenCharacter>(MeshComp->GetOwner()))
	{
		Fremen->AttackReset();
	}
}
