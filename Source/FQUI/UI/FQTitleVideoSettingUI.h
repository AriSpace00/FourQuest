// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQTitleVideoSettingUI.generated.h"

UENUM()
enum class ETitleVideoSettingButtonType : uint8
{
	Resolution,					// 해상도
	FullScreen,					// 전체화면
	WindowScreen,				// 창모드
	Apply,						// 적용
	End
};

UCLASS()
class FQUI_API UFQTitleVideoSettingUI : public UFQUserWidget
{
	GENERATED_BODY()

public:
	UFQTitleVideoSettingUI();

public:
	// Delegate
	FQ_UIDelegate::FQSettingUIDelegate SettingUIDelegate;

	// Input Function
	void Move(EWidgetInputType InputType);
	void SelectButton();
	void CancelButton();
	void PlayAnimation(float DeltaTime);
	void InitSelect();

protected:
	// Parent Function
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation)
	TArray<TObjectPtr<UTexture2D>> mSoulAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQText)
	TMap<ETitleVideoSettingButtonType, FText> mGuides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQCheckBox)
	TArray<TObjectPtr<UTexture2D>> mCheckBoxs;

private:
	// 현재 선택
	ETitleVideoSettingButtonType mCurrentSelectIndex;

	// 현재 설정 내용
	FQ_InGameSetting::FVideoInfomation mCurrentVideoInfomation;

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
	TObjectPtr<class UImage> mFullScreenCheck;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mWindowScreenCheck;

	// 해상도 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mResolutionText;
};
