// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQSoulDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQSOUL_API UFQSoulDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQSoulDataAsset();

	UPROPERTY(EditAnywhere, Category = Walk, meta = (ToolTip = "영혼 기본 속도"))
	float	mWalkSpeed;

	UPROPERTY(EditAnywhere, Category = Dash, meta = (ToolTip = "영혼 대쉬 지속시간"))
	float	mDashDuration;
	UPROPERTY(EditAnywhere, Category = Dash, meta = (ToolTip = "영혼 대쉬 쿨타임"))
	float	mDashCoolTime;
	UPROPERTY(EditAnywhere, Category = Dash, meta = (ToolTip = "영혼 대쉬 속도"))
	float	mDashSpeed;
};
