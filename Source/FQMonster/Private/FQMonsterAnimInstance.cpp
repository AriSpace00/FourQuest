// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMonsterAnimInstance.h"

void UFQMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!mOwnerPawn)
	{
		mOwnerPawn = TryGetPawnOwner();
	}
	AFQMonsterBase* Monster = Cast<AFQMonsterBase>(mOwnerPawn);
	if (Monster)
	{
		mCurrentState = Monster->mMonsterState;
	}
}
