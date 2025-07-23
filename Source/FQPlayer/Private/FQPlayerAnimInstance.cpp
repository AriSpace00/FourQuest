// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerAnimInstance.h"
#include "FQPlayer/Public/FQPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UFQPlayerAnimInstance::UFQPlayerAnimInstance()
{
	mMovingThreshould = 3.0f;
}

void UFQPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	mOwner = Cast<AFQPlayerBase>(GetOwningActor());
	if (mOwner)
	{
		mMovement = Cast<UCharacterMovementComponent>(mOwner->GetCharacterMovement());
	}
}

void UFQPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!mOwner)
	{
		return;
	}

	if (!mMovement)
	{
		return;
	}

	// Speed
	mVelocity = mMovement->Velocity;
	mGroundSpeed = mVelocity.Size2D();
	mNormalizedSpeed = mGroundSpeed / mMovement->GetMaxSpeed();
	mbIsIdle = mGroundSpeed < mMovingThreshould;
	mbHitReacting = mOwner->IsHit();
}


