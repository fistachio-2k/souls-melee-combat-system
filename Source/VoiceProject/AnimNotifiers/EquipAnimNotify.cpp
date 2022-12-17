// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipAnimNotify.h"

#include "VoiceProject/Characters/FremenCharacter.h"
#include "VoiceProject/Items/BaseWeapon.h"

void UEquipAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (const AFremenCharacter* Character = Cast<AFremenCharacter>(Owner))
		{
			ABaseWeapon& Weapon = Character->GetMainWeapon();
			Weapon.AttachActor(Weapon.IsWeaponInHande() ? Weapon.HeapSocketName : Weapon.HandSocketName);
		}
	}
}
