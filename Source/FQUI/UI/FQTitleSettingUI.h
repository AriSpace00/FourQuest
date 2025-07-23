// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQTitleSettingUI.generated.h"

UENUM()
enum class ETitleSettingButtonType : uint8
{
	Video,
	Audio,
	Back,
	End
};

UCLASS()
class FQUI_API UFQTitleSettingUI : public UFQUserWidget
{
	GENERATED_BODY()

public:
	UFQTitleSettingUI();

public:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQAnimation, Meta = (AllowPrivateAccess = "true"))
	TMap<ETitleSettingButtonType, FText> mGuides;

private:
	// 현재 선택
	ETitleSettingButtonType mCurrentSelectIndex;

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
	TObjectPtr<class UImage> mSelectButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectAnimation;
};
