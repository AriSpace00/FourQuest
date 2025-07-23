// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_EnableVolume.h"
#include "FQPlayer/Public/FQPlayerBase.h"

void UAnimNotifyState_EnableVolume::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AFQPlayerBase* Player = Cast<AFQPlayerBase>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->EnableAttackVolume();
}

void UAnimNotifyState_EnableVolume::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AFQPlayerBase* Player = Cast<AFQPlayerBase>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->DisableAttackVolume();
}
