// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQInGameSettingWidget.generated.h"

UENUM()
enum class EGameSettingButtonType : uint8
{
	Continue,
	Setting,
	Failed,
	Logout,
};

UCLASS()
class FQUI_API UFQInGameSettingWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQInGameSettingWidget();

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
	TMap<EGameSettingButtonType, FText> mGuides;

private:
	float mOriginYPos;
	float mElapsedTime;
	uint8 mCurrentFrameIndex;

	EGameSettingButtonType mCurrentButtonType;

	// 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSelectAnimation;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mGuideText;
};
