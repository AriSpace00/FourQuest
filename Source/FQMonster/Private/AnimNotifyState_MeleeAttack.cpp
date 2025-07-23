// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_MeleeAttack.h"
#include "FQMonsterBase.h"
#include "AIController.h"

void UAnimNotifyState_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AFQMonsterBase* Monster = Cast<AFQMonsterBase>(MeshComp->GetOwner());
	if (Monster)
	{
		AAIController* AIC = Monster->GetController<AAIController>();
		AIC->ClearFocus(EAIFocusPriority::LastFocusPriority);
		Monster->SetCollisionEnabled(true);
		Monster->ApplyDamageToTarget();
	}
}

void UAnimNotifyState_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AFQMonsterBase* Monster = Cast<AFQMonsterBase>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->SetCollisionEnabled(false);
	}
}
