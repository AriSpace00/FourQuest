// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API UFQMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQMonsterDataAsset();

	UPROPERTY(EditAnywhere,Category="Stat")
	float mMaxHP;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float mAttackPower;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mChaseSpeed;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mAttackRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mPatrolDistance;
};
