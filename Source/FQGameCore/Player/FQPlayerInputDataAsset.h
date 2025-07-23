// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQPlayerInputDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQGAMECORE_API UFQPlayerInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQPlayerInputDataAsset();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mLeftStickAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mRightStickAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mXButtonAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mAButtonAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mBButtonAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mYButtonAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputAction> mMenuButtonAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FQInput)
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;
};
