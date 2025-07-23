// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMageAnimInstance.h"
#include "FQPlayer/Public/FQMagePlayer.h"

UFQMageAnimInstance::UFQMageAnimInstance()
{
	mbIsEnabledCircle = false;
}

void UFQMageAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UFQMageAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!mOwner)
	{
		return;
	}

	AFQMagePlayer* Player = Cast<AFQMagePlayer>(mOwner);
	if (!Player)
	{
		return;
	}

	mbIsEnabledCircle = Player->IsEnabledExplosionCircle();
	mbIsEnabledLaser = Player->IsEnabledLaser();
}
