// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FQMonsterBase.h"
#include "FQMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API UFQMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EMonsterState mCurrentState;

private:
	APawn* mOwnerPawn;
};
