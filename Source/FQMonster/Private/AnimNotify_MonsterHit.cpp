// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterHit.h"
#include "FQMonsterBase.h"

void UAnimNotify_MonsterHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AFQMonsterBase* Monster = Cast<AFQMonsterBase>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->ChangeState(EMonsterState::Chase);
	}
}
