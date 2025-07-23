// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQTitleScreenWidget.generated.h"

UENUM()
enum class ETitleButtonType : uint8
{
	GameStart = 0,
	GameSetting,
	GameExit,
	End
};

UENUM()
enum class EWidgetLayer : uint8
{
	FirstLayer = 0,
	SettingLayer,
	End
};

UCLASS()
class FQUI_API UFQTitleScreenWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQTitleScreenWidget();

	// Input Function
	void WidgetInput(EWidgetInputType InputType);
	void TitleSetting(ESettingUIType SettingType);

	// GetSet Function
	FORCEINLINE ETitleButtonType GetSelectIndex() const { return mCurrentSelectIndex; }

protected:
	// Parent Function
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Common Function
	void MoveIndex(EWidgetInputType InputType);
	void Move(EWidgetInputType MoveType);
	void SelectButton();
	void CancelButton();
	void TitleSelectButton();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation)
	TArray<TObjectPtr<UTexture2D>> mSoulAnimations;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCurrentSelect;

private:
	float mElapsedTime;
	uint8 mCurrentFrameIndex;

	ETitleButtonType mCurrentSelectIndex;
	ESettingUIType mCurrentSettingType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleSettingUI> mTitleSettingUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleVideoSettingUI> mTitleVideoSettingUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleAudioSettingUI> mTitleAudioSettingUI;
};
