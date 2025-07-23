// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_BossAttack.h"
#include "FQBossMonster.h"
#include "AIController.h"

void UAnimNotifyState_BossAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AFQBossMonster* Monster = Cast<AFQBossMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		AAIController* AIC = Monster->GetController<AAIController>();
		AIC->ClearFocus(EAIFocusPriority::LastFocusPriority);
		Monster->SetCollisionEnabled(true);
		
	}
}

void UAnimNotifyState_BossAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AFQBossMonster* Monster = Cast<AFQBossMonster>(MeshComp->GetOwner());
	if (Monster)
	{
		Monster->ApplyDamageToTarget();
		Monster->SetCollisionEnabled(false);
	}
}
