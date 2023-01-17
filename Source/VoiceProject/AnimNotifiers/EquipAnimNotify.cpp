// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipAnimNotify.h"

#include "VoiceProject/Characters/FremenCharacter.h"
#include "VoiceProject/Components/CombatComponent.h"
#include "VoiceProject/Items/BaseWeapon.h"

void UEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (const AFremenCharacter* Character = Cast<AFremenCharacter>(Owner))
		{
			// TODO: Consider make a getter for the CombatComponent to avoid casting
			ABaseWeapon* Weapon = nullptr;
			UActorComponent* ActorComponent = Character->GetComponentByClass(TSubclassOf<UCombatComponent>());
			if (const UCombatComponent* CombatComponent = Cast<UCombatComponent>(ActorComponent))
			{
				Weapon = CombatComponent->GetMainWeapon();
			}
			
			Weapon->AttachActor(Weapon->IsWeaponInHand() ? Weapon->HeapSocketName : Weapon->HandSocketName);
		}
	}
}
