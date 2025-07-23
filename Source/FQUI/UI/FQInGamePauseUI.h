// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQInGamePauseUI.generated.h"

DECLARE_DELEGATE(FQInGamePauseUIDelegate);

UCLASS()
class FQUI_API UFQInGamePauseUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQInGamePauseUI();

public:
	FQInGamePauseUIDelegate mPauseUIDelegate;

	// 입력 함수
	void InitUI();
	void WidgetInput(EWidgetInputType InputType);

protected:
	// 부모 가상 함수
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	// 일반 함수
	void MoveIndex(EWidgetInputType InputType);
	void SelectButton();
	void CancelButton();

	// 콜백 함수
	void SettingUI(ESettingUIType SettingType);

private:
	ESettingUIType mCurrentSettingType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQInGameSettingWidget> mPauseUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleSettingUI> mSettingUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleVideoSettingUI> mVideoSettingUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFQTitleAudioSettingUI> mAudioSettingUI;
};
