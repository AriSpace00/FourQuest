// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQSoulSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulSelectUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQSoulSelectUI();

	// Common Function
	void UpdateSoulAnimation(int32 AnimationIndex);
	void UpdateSoulType(ESoulType NewSoulType);

protected:
	// Parent Function
	virtual void NativeConstruct() override;

private:
	float mElapsedTime;
	ESoulType mCurrentSoulType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTexture2D>> mSwordSoulAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTexture2D>> mStaffSoulAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTexture2D>> mAxeSoulAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTexture2D>> mBowSoulAnimations;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQCheck, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> mCheckIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQCheck, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> mEmptyIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSoulIndex4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<ESoulType, FText> mSoulNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<ESoulType, FText> mSoulInfomations;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mSoulName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mSoulInfomation;
};
