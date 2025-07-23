#include "FQInGamePauseUI.h"

#include "FQInGameSettingWidget.h"
#include "FQTitleAudioSettingUI.h"
#include "FQTitleVideoSettingUI.h"
#include "FQTitleSettingUI.h"

UFQInGamePauseUI::UFQInGamePauseUI()
	:mCurrentSettingType(ESettingUIType::None)
{
}

void UFQInGamePauseUI::NativeConstruct()
{
	Super::NativeConstruct();

	mPauseUI->SettingUIDelegate.BindUObject(this, &UFQInGamePauseUI::SettingUI);
	mSettingUI->SettingUIDelegate.BindUObject(this, &UFQInGamePauseUI::SettingUI);
	mVideoSettingUI->SettingUIDelegate.BindUObject(this, &UFQInGamePauseUI::SettingUI);
	mAudioSettingUI->SettingUIDelegate.BindUObject(this, &UFQInGamePauseUI::SettingUI);
}

void UFQInGamePauseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	switch (mCurrentSettingType)
	{
	case ESettingUIType::None:
		mPauseUI->PlayAnimation(InDeltaTime);
		break;
	case ESettingUIType::MainSetting:
		mSettingUI->PlayAnimation(InDeltaTime);
		break;
	case ESettingUIType::VideoSetting:
		mVideoSettingUI->PlayAnimation(InDeltaTime);
		break;
	case ESettingUIType::AudioSetting:
		mAudioSettingUI->PlayAnimation(InDeltaTime);
		break;
	}
}

void UFQInGamePauseUI::InitUI()
{
	SettingUI(ESettingUIType::None);
}

void UFQInGamePauseUI::WidgetInput(EWidgetInputType InputType)
{
	switch (InputType)
	{
	case EWidgetInputType::Left:
		MoveIndex(EWidgetInputType::Left);
		break;
	case EWidgetInputType::Right:
		MoveIndex(EWidgetInputType::Right);
		break;
	case EWidgetInputType::Up:
		MoveIndex(EWidgetInputType::Up);
		break;
	case EWidgetInputType::Down:
		MoveIndex(EWidgetInputType::Down);
		break;
	case EWidgetInputType::Select:
		SelectButton();
		break;
	case EWidgetInputType::Cancel:
		CancelButton();
		break;
	}
}

void UFQInGamePauseUI::MoveIndex(EWidgetInputType InputType)
{
	if (!mPauseUI || !mSettingUI || !mVideoSettingUI || !mAudioSettingUI)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mPauseUI, mSettingUI, mVideoSettingUI, mAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	switch (mCurrentSettingType)
	{
	case ESettingUIType::None:
		mPauseUI->Move(InputType);
		break;
	case ESettingUIType::MainSetting:
		mSettingUI->Move(InputType);
		break;
	case ESettingUIType::VideoSetting:
		mVideoSettingUI->Move(InputType);
		break;
	case ESettingUIType::AudioSetting:
		mAudioSettingUI->Move(InputType);
		break;
	}
}

void UFQInGamePauseUI::SelectButton()
{
	if (!mPauseUI || !mSettingUI || !mVideoSettingUI || !mAudioSettingUI)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mPauseUI, mSettingUI, mVideoSettingUI, mAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	switch (mCurrentSettingType)
	{
	case ESettingUIType::None:
		mPauseUI->SelectButton();
		break;
	case ESettingUIType::MainSetting:
		mSettingUI->SelectButton();
		break;
	case ESettingUIType::VideoSetting:
		mVideoSettingUI->SelectButton();
		break;
	case ESettingUIType::AudioSetting:
		mAudioSettingUI->SelectButton();
		break;
	}
}

void UFQInGamePauseUI::CancelButton()
{
	if (!mPauseUI || !mSettingUI || !mVideoSettingUI || !mAudioSettingUI)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mPauseUI, mSettingUI, mVideoSettingUI, mAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	switch (mCurrentSettingType)
	{
	case ESettingUIType::None:
		mPauseUI->CancelButton();
		break;
	case ESettingUIType::MainSetting:
		mSettingUI->CancelButton();
		break;
	case ESettingUIType::VideoSetting:
		mVideoSettingUI->CancelButton();
		break;
	case ESettingUIType::AudioSetting:
		mAudioSettingUI->CancelButton();
		break;
	}
}

void UFQInGamePauseUI::SettingUI(ESettingUIType SettingType)
{
	if (!mPauseUI || !mSettingUI || !mVideoSettingUI || !mAudioSettingUI)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQInGamePauseUI %d] mPauseUI, mSettingUI, mVideoSettingUI, mAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	mCurrentSettingType = SettingType;
	switch (mCurrentSettingType)
	{
	case ESettingUIType::None:
	{
		mPauseUI->InitSelect();

		mPauseUI->SetVisibility(ESlateVisibility::Visible);
		mSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case ESettingUIType::MainSetting:
	{
		mSettingUI->InitSelect();

		mPauseUI->SetVisibility(ESlateVisibility::Hidden);
		mSettingUI->SetVisibility(ESlateVisibility::Visible);
		mVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case ESettingUIType::VideoSetting:
	{
		mVideoSettingUI->InitSelect();

		mPauseUI->SetVisibility(ESlateVisibility::Hidden);
		mSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mVideoSettingUI->SetVisibility(ESlateVisibility::Visible);
		mAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case ESettingUIType::AudioSetting:
	{
		mAudioSettingUI->InitSelect();

		mPauseUI->SetVisibility(ESlateVisibility::Hidden);
		mSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
		mAudioSettingUI->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case ESettingUIType::End:
		mPauseUIDelegate.ExecuteIfBound();
		break;
	}
}
