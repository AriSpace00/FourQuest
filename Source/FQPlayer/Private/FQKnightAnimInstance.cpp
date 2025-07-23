// Fill out your copyright notice in the Description page of Project Settings.


#include "FQKnightAnimInstance.h"
#include "FQPlayer/Public/FQKnightPlayer.h"

UFQKnightAnimInstance::UFQKnightAnimInstance()
{
	mbIsHoldShield = false;
}

void UFQKnightAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UFQKnightAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!mOwner)
	{
		return;
	}

	AFQKnightPlayer* Player = Cast<AFQKnightPlayer>(mOwner);
	if (!Player)
	{
		return;
	}

	mShieldMoveAngle = Player->GetShieldMoveAngle();
	mbIsHoldShield = Player->IsHoldShield();
}