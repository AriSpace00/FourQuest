// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQTitleAudioSettingUI.generated.h"

UENUM()
enum class ETitleAudioSettingButtonType
{
	FullVolum,
	BGMVolum,
	SFXVolum,
	Apply
};

UCLASS()
class FQUI_API UFQTitleAudioSettingUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQTitleAudioSettingUI();

public:
	FQ_UIDelegate::FQSettingUIDelegate SettingUIDelegate;

	// Input Function
	void InitSelect();
	void Move(EWidgetInputType InputType);
	void SelectButton();
	void CancelButton();
	void PlayAnimation(float DeltaTime);

	// 
	FQ_InGameSetting::FAudioInfomation GetAudioInfomation() const { return mCurrentAudioInfomation; }
	void SetAudioInfomation(const FQ_InGameSetting::FAudioInfomation AudioInfomation) { mCurrentAudioInfomation = AudioInfomation; }

protected:
	// Parent Function
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation)
	TArray<TObjectPtr<UTexture2D>> mSoulAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQText)
	TMap<ETitleAudioSettingButtonType, FText> mGuides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQCheckBox)
	TArray<TObjectPtr<UTexture2D>> mCheckBoxs;

private:
	// 현재 선택
	ETitleAudioSettingButtonType mCurrentSelectIndex;

	// 현재 설정 내용
	FQ_InGameSetting::FAudioInfomation mCurrentAudioInfomation;

	// 처음 선택 버튼 위치
	float mOriginYPos;

	// 애니메이션
	float mElapsedTime;
	uint8 mCurrentFrameIndex;

	// 가이드 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mTextGuide;

	// 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCurrentSelect;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectAnimation;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mApplyButton;

	// 체크 박스
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckBoxFullMute;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckBoxBGMMute;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCheckBoxSFXMute;

	// 슬라이더
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> mFullVolumSlider;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> mBGMVolumSlider;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> mSFXVolumSlider;

	// 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mFullVolumPercentText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mBGMVolumPercentText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mSFXVolumPercentText;
};
