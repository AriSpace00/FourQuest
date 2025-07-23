// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"
#include "FQRangeMonster.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFQRangeMonster* Monster = Cast<AFQRangeMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->ProjectileAttack();
	}
}
