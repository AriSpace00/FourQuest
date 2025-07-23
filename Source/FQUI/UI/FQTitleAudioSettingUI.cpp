#include "FQTitleAudioSettingUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components\CanvasPanelSlot.h"
#include "FQGameCore\GameInstance\FQGameInstanceInterface.h"


UFQTitleAudioSettingUI::UFQTitleAudioSettingUI()
	: mElapsedTime()
	, mCurrentFrameIndex()
{
	mGuides.Add(ETitleAudioSettingButtonType::FullVolum, FText::FromString(TEXT("게임 내 모든 사운드 음량을 조절합니다. 슬라이더로 조작합니다.")));
	mGuides.Add(ETitleAudioSettingButtonType::BGMVolum, FText::FromString(TEXT("게임 내 배경 사운드 음량을 조절합니다. 슬라이더로 조작합니다.")));
	mGuides.Add(ETitleAudioSettingButtonType::SFXVolum, FText::FromString(TEXT("게임 내 효과음 사운드 음량을 조절합니다. 슬라이더로 조작합니다.")));
	mGuides.Add(ETitleAudioSettingButtonType::Apply, FText::FromString(TEXT("저장한 오디오 정보를 세팅하고 메인 세팅 화면으로 돌아갑니다.")));
}

void UFQTitleAudioSettingUI::NativeConstruct()
{
	Super::NativeConstruct();

	mApplyButton->SetVisibility(ESlateVisibility::Hidden);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
	{
		mOriginYPos = CanvasSlot->GetPosition().Y;
	}
}

void UFQTitleAudioSettingUI::InitSelect()
{
	// UI 초기화
	mCurrentSelectIndex = ETitleAudioSettingButtonType::FullVolum;
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}
	mCurrentSelect->SetVisibility(ESlateVisibility::Visible);
	mSelectAnimation->SetVisibility(ESlateVisibility::Visible);
	mApplyButton->SetVisibility(ESlateVisibility::Hidden);

	mTextGuide->SetText(mGuides[mCurrentSelectIndex]);

	// 오디오 정보 저장
	IFQGameInstanceInterface* MyGameInstance = Cast<IFQGameInstanceInterface>(GetWorld()->GetGameInstance());
	if (MyGameInstance)
	{
		mCurrentAudioInfomation = MyGameInstance->GetAudioInfomation();

		// 음소거 체크박스 세팅
		mCheckBoxFullMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsFullVolumMute]);
		mCheckBoxBGMMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsBGMVolumMute]);
		mCheckBoxSFXMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsSFXVolumMute]);

		// 슬라이더 세팅
		mFullVolumSlider->SetValue(mCurrentAudioInfomation.mFullVolumValue);
		mBGMVolumSlider->SetValue(mCurrentAudioInfomation.mBGMVolumValue);
		mSFXVolumSlider->SetValue(mCurrentAudioInfomation.mSFXVolumValue);

		// 텍스트 세팅
		int32 VolumePercent = (int32)(mCurrentAudioInfomation.mFullVolumValue * 100);
		FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
		mFullVolumPercentText->SetText(SettingText);
		VolumePercent = (int32)(mCurrentAudioInfomation.mBGMVolumValue * 100);
		SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
		mBGMVolumPercentText->SetText(SettingText);
		VolumePercent = (int32)(mCurrentAudioInfomation.mSFXVolumValue * 100);
		SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
		mSFXVolumPercentText->SetText(SettingText);
	}
}

void UFQTitleAudioSettingUI::Move(EWidgetInputType InputType)
{
	if (mCurrentSelect == nullptr || mSelectAnimation == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Is nullptr!!"), __LINE__);
		return;
	}

	if (InputType == EWidgetInputType::Up && mCurrentSelectIndex != ETitleAudioSettingButtonType::FullVolum && mCurrentSelectIndex < ETitleAudioSettingButtonType::Apply)
	{
		mCurrentSelectIndex = (ETitleAudioSettingButtonType)((uint8)(mCurrentSelectIndex)-1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0,-120));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -120));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex < ETitleAudioSettingButtonType::SFXVolum)
	{
		mCurrentSelectIndex = (ETitleAudioSettingButtonType)((uint8)mCurrentSelectIndex + 1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 120));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 120));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex == ETitleAudioSettingButtonType::SFXVolum)
	{
		mCurrentSelectIndex = ETitleAudioSettingButtonType::Apply;

		mCurrentSelect->SetVisibility(ESlateVisibility::Hidden);
		mSelectAnimation->SetVisibility(ESlateVisibility::Hidden);
		mApplyButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (InputType == EWidgetInputType::Up && mCurrentSelectIndex == ETitleAudioSettingButtonType::Apply)
	{
		mCurrentSelectIndex = ETitleAudioSettingButtonType::SFXVolum;

		mCurrentSelect->SetVisibility(ESlateVisibility::Visible);
		mSelectAnimation->SetVisibility(ESlateVisibility::Visible);
		mApplyButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InputType == EWidgetInputType::Left)
	{
		switch (mCurrentSelectIndex)
		{
		case ETitleAudioSettingButtonType::FullVolum:
		{
			mCurrentAudioInfomation.mFullVolumValue = mCurrentAudioInfomation.mFullVolumValue - 0.05f < 0.f ? 0.f : mCurrentAudioInfomation.mFullVolumValue - 0.05f;
			mFullVolumSlider->SetValue(mCurrentAudioInfomation.mFullVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mFullVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mFullVolumPercentText->SetText(SettingText);
		}
		break;
		case ETitleAudioSettingButtonType::BGMVolum:
		{
			mCurrentAudioInfomation.mBGMVolumValue = mCurrentAudioInfomation.mBGMVolumValue - 0.05f < 0.f ? 0.f : mCurrentAudioInfomation.mBGMVolumValue - 0.05f;
			mBGMVolumSlider->SetValue(mCurrentAudioInfomation.mBGMVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mBGMVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mBGMVolumPercentText->SetText(SettingText);
		}
		break;
		case ETitleAudioSettingButtonType::SFXVolum:
		{
			mCurrentAudioInfomation.mSFXVolumValue = mCurrentAudioInfomation.mSFXVolumValue - 0.05f < 0.f ? 0.f : mCurrentAudioInfomation.mSFXVolumValue - 0.05f;
			mSFXVolumSlider->SetValue(mCurrentAudioInfomation.mSFXVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mSFXVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mSFXVolumPercentText->SetText(SettingText);
		}
		break;
		}
	}
	else if (InputType == EWidgetInputType::Right)
	{
		switch (mCurrentSelectIndex)
		{
		case ETitleAudioSettingButtonType::FullVolum:
		{
			mCurrentAudioInfomation.mFullVolumValue = mCurrentAudioInfomation.mFullVolumValue + 0.05f > 1.f ? 1.f : mCurrentAudioInfomation.mFullVolumValue + 0.05f;
			mFullVolumSlider->SetValue(mCurrentAudioInfomation.mFullVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mFullVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mFullVolumPercentText->SetText(SettingText);
		}
		break;
		case ETitleAudioSettingButtonType::BGMVolum:
		{
			mCurrentAudioInfomation.mBGMVolumValue = mCurrentAudioInfomation.mBGMVolumValue + 0.05f > 1.f ? 1.f : mCurrentAudioInfomation.mBGMVolumValue + 0.05f;
			mBGMVolumSlider->SetValue(mCurrentAudioInfomation.mBGMVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mBGMVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mBGMVolumPercentText->SetText(SettingText);
		}
		break;
		case ETitleAudioSettingButtonType::SFXVolum:
		{
			mCurrentAudioInfomation.mSFXVolumValue = mCurrentAudioInfomation.mSFXVolumValue + 0.05f > 1.f ? 1.f : mCurrentAudioInfomation.mSFXVolumValue + 0.05f;
			mSFXVolumSlider->SetValue(mCurrentAudioInfomation.mSFXVolumValue);
			int32 VolumePercent = (int32)(mCurrentAudioInfomation.mSFXVolumValue * 100);
			FText SettingText = FText::FromString(FString::Printf(TEXT("%d%%"), VolumePercent));
			mSFXVolumPercentText->SetText(SettingText);
		}
		break;
		}
	}

	// 설명 텍스트 세팅
	mTextGuide->SetText(mGuides[mCurrentSelectIndex]);
}

void UFQTitleAudioSettingUI::SelectButton()
{
	switch (mCurrentSelectIndex)
	{
	case ETitleAudioSettingButtonType::FullVolum:
	{
		mCurrentAudioInfomation.mbIsFullVolumMute = mCurrentAudioInfomation.mbIsFullVolumMute ? false : true;
		mCheckBoxFullMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsFullVolumMute]);
	}
	break;
	case ETitleAudioSettingButtonType::BGMVolum:
	{
		mCurrentAudioInfomation.mbIsBGMVolumMute = mCurrentAudioInfomation.mbIsBGMVolumMute ? false : true;
		mCheckBoxBGMMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsBGMVolumMute]);
	}
	break;
	case ETitleAudioSettingButtonType::SFXVolum:
	{
		mCurrentAudioInfomation.mbIsSFXVolumMute = mCurrentAudioInfomation.mbIsSFXVolumMute ? false : true;
		mCheckBoxSFXMute->SetBrushFromTexture(mCheckBoxs[mCurrentAudioInfomation.mbIsSFXVolumMute]);
	}
	break;
	case ETitleAudioSettingButtonType::Apply:
	{
		// 오디오 정보 저장
		IFQGameInstanceInterface* MyGameInstance = Cast<IFQGameInstanceInterface>(GetWorld()->GetGameInstance());
		if (MyGameInstance)
		{
			MyGameInstance->SetAudioInfomation(mCurrentAudioInfomation);
		}

		// 메인 설정 창으로 돌아가기
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::MainSetting);
	}
	break;
	}
}

void UFQTitleAudioSettingUI::CancelButton()
{
	SettingUIDelegate.ExecuteIfBound(ESettingUIType::MainSetting);
}

void UFQTitleAudioSettingUI::PlayAnimation(float DeltaTime)
{
	mElapsedTime += DeltaTime;
	if (mElapsedTime < 0.1f)
	{
		return;
	}

	// 애니메이션 플레이
	mElapsedTime = 0.f;
	if (mSoulAnimations.Num() > 0)
	{
		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mSoulAnimations.Num();
		if (mSelectAnimation)
		{
			mSelectAnimation->SetBrushFromTexture(mSoulAnimations[mCurrentFrameIndex]);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mCurrentSelect Is Nullptr!!"), __LINE__);
		}
	}
}
