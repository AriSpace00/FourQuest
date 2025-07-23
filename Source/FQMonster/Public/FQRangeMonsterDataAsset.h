// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQRangeMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API UFQRangeMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float mMaxHP;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mAttackPower;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mAttackRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float mProjectileSpeed;
};
