// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQPlayerHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQPlayerHpWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQPlayerHpWidget();

	// Common Function
	void UpdateHpDecrasePercent(float InDeltaTime);

	// Callback Function
	void UpdatePlayerHp(float HpValue);
	void UpdatePlayerControllerNumber(int32 PlayerControllerNumber, ESoulType SoulType);
	void SetHpBarVisibility(bool bIsVisible);

private:
	// Parent Function
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQPlayer)
	TMap<ESoulType, TSoftObjectPtr<UTexture2D>> mPlayerMap1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQPlayer)
	TMap<ESoulType, TSoftObjectPtr<UTexture2D>> mPlayerMap2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQPlayer)
	TMap<ESoulType, TSoftObjectPtr<UTexture2D>> mPlayerMap3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQPlayer)
	TMap<ESoulType, TSoftObjectPtr<UTexture2D>> mPlayerMap4;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mPlayerNumber;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mHpBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mHp;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mHpDecrase;

	float mHpPercent;
	float mHpDecrasePercent;
};
