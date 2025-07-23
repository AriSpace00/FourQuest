// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQBossMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API UFQBossMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float mMaxHP;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float mAttackPower;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float mStrikeAttackPower;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mChaseSpeed;

	UPROPERTY(EditAnywhere, Category = "Rush")
	float mRushSpeed;

	UPROPERTY(EditAnywhere, Category = "Rush")
	float mRushDamage;

	UPROPERTY(EditAnywhere, Category = "Rush")
	float mRushStrength;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float mAttackRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mDownHitDamage;

	UPROPERTY(EditAnywhere, Category = "SPP")
	float mInitSPP;

	UPROPERTY(EditAnywhere, Category = "SPP")
	float mHitSPP;

	UPROPERTY(EditAnywhere, Category = "SPP")
	float mAttackSPP;

	UPROPERTY(EditAnywhere, Category = "SPP")
	float mTimeSPP;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mTargetingTime;
};
