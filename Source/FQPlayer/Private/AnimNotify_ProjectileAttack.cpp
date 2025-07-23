// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/AnimNotify_ProjectileAttack.h"
#include "FQPlayer/Public/FQMagePlayer.h"

void UAnimNotify_ProjectileAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	AFQMagePlayer* Player = Cast<AFQMagePlayer>(MeshComp->GetOwner());
	if (!Player)
	{
		return;
	}

	Player->ProcessProjectileAttack();
}
