// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/AnimNotify_ProcessNextSection.h"
#include "FQPlayer/Public/FQPlayerBase.h"

void UAnimNotify_ProcessNextSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	AFQPlayerBase* Player = Cast<AFQPlayerBase>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->ProcessNextSection();
}
