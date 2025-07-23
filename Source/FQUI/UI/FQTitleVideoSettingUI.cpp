#include "FQTitleVideoSettingUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components\CanvasPanelSlot.h"
#include "FQGameCore\GameInstance\FQGameInstanceInterface.h"


UFQTitleVideoSettingUI::UFQTitleVideoSettingUI()
{
	mGuides.Add(ETitleVideoSettingButtonType::Resolution, FText::FromString(TEXT("해상도를 설정합니다. 슬라이더로 조작합니다.")));
	mGuides.Add(ETitleVideoSettingButtonType::FullScreen, FText::FromString(TEXT("전체 화면으로 전환합니다.")));
	mGuides.Add(ETitleVideoSettingButtonType::WindowScreen, FText::FromString(TEXT("창모드로 전환합니다.")));
	mGuides.Add(ETitleVideoSettingButtonType::Apply, FText::FromString(TEXT("게임 화면 설정을 적용하고 메인 설정 화면으로 돌아갑니다.")));
}

void UFQTitleVideoSettingUI::NativeConstruct()
{
	Super::NativeConstruct();

	mApplyButton->SetVisibility(ESlateVisibility::Hidden);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
	{
		mOriginYPos = CanvasSlot->GetPosition().Y;
	}
}

void UFQTitleVideoSettingUI::Move(EWidgetInputType InputType)
{
	if (mCurrentSelect == nullptr || mSelectAnimation == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Is nullptr!!"), __LINE__);
		return;
	}

	if (InputType == EWidgetInputType::Up && mCurrentSelectIndex != ETitleVideoSettingButtonType::Resolution && mCurrentSelectIndex < ETitleVideoSettingButtonType::Apply)
	{
		mCurrentSelectIndex = (ETitleVideoSettingButtonType)((uint8)(mCurrentSelectIndex) - 1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 
				mCurrentSelectIndex == ETitleVideoSettingButtonType::Resolution ? -150 : -100));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0,
				mCurrentSelectIndex == ETitleVideoSettingButtonType::Resolution ? -150 : -100));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex < ETitleVideoSettingButtonType::WindowScreen)
	{
		mCurrentSelectIndex = (ETitleVideoSettingButtonType)((uint8)mCurrentSelectIndex + 1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0,
				mCurrentSelectIndex == ETitleVideoSettingButtonType::FullScreen ? 150 : 100));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0,
				mCurrentSelectIndex == ETitleVideoSettingButtonType::FullScreen ? 150 : 100));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex == ETitleVideoSettingButtonType::WindowScreen)
	{
		mCurrentSelectIndex = ETitleVideoSettingButtonType::Apply;

		mCurrentSelect->SetVisibility(ESlateVisibility::Hidden);
		mSelectAnimation->SetVisibility(ESlateVisibility::Hidden);
		mApplyButton->SetVisibility(ESlateVisibility::Visible);
	}
	else if (InputType == EWidgetInputType::Up && mCurrentSelectIndex == ETitleVideoSettingButtonType::Apply)
	{
		mCurrentSelectIndex = ETitleVideoSettingButtonType::WindowScreen;

		mCurrentSelect->SetVisibility(ESlateVisibility::Visible);
		mSelectAnimation->SetVisibility(ESlateVisibility::Visible);
		mApplyButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InputType == EWidgetInputType::Left && mCurrentSelectIndex == ETitleVideoSettingButtonType::Resolution)
	{
		mCurrentVideoInfomation.mCurrentResolutionType = mCurrentVideoInfomation.mCurrentResolutionType == FQ_InGameSetting::EResolutionType::Resolution_1080x600 ?
			FQ_InGameSetting::EResolutionType::Resolution_1080x600 : (FQ_InGameSetting::EResolutionType)((int32)mCurrentVideoInfomation.mCurrentResolutionType - 1);

		switch (mCurrentVideoInfomation.mCurrentResolutionType)
		{
		case FQ_InGameSetting::EResolutionType::Resolution_1920x1080:
			mResolutionText->SetText(FText::FromString(TEXT("1920 x 1080")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1600x900:
			mResolutionText->SetText(FText::FromString(TEXT("1600 x 900")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1280x720:
			mResolutionText->SetText(FText::FromString(TEXT("1280 x 720")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1080x600:
			mResolutionText->SetText(FText::FromString(TEXT("1080 x 600")));
			break;
		}
	}
	else if (InputType == EWidgetInputType::Right && mCurrentSelectIndex == ETitleVideoSettingButtonType::Resolution)
	{
		mCurrentVideoInfomation.mCurrentResolutionType = mCurrentVideoInfomation.mCurrentResolutionType == FQ_InGameSetting::EResolutionType::Resolution_1920x1080 ?
			FQ_InGameSetting::EResolutionType::Resolution_1920x1080 : (FQ_InGameSetting::EResolutionType)((int32)mCurrentVideoInfomation.mCurrentResolutionType + 1);

		switch (mCurrentVideoInfomation.mCurrentResolutionType)
		{
		case FQ_InGameSetting::EResolutionType::Resolution_1920x1080:
			mResolutionText->SetText(FText::FromString(TEXT("1920 x 1080")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1600x900:
			mResolutionText->SetText(FText::FromString(TEXT("1600 x 900")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1280x720:
			mResolutionText->SetText(FText::FromString(TEXT("1280 x 720")));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1080x600:
			mResolutionText->SetText(FText::FromString(TEXT("1080 x 600")));
			break;
		}
	}

	// 설명 텍스트 세팅
	mTextGuide->SetText(mGuides[mCurrentSelectIndex]);
}

void UFQTitleVideoSettingUI::SelectButton()
{
	switch (mCurrentSelectIndex)
	{
	case ETitleVideoSettingButtonType::FullScreen:
	{
		mCurrentVideoInfomation.mbIsFullScreen = true;
		mFullScreenCheck->SetBrushFromTexture(mCheckBoxs[mCurrentVideoInfomation.mbIsFullScreen]);
		mWindowScreenCheck->SetBrushFromTexture(mCheckBoxs[!mCurrentVideoInfomation.mbIsFullScreen]);
	}
	break;
	case ETitleVideoSettingButtonType::WindowScreen:
	{
		mCurrentVideoInfomation.mbIsFullScreen = false;
		mFullScreenCheck->SetBrushFromTexture(mCheckBoxs[mCurrentVideoInfomation.mbIsFullScreen]);
		mWindowScreenCheck->SetBrushFromTexture(mCheckBoxs[!mCurrentVideoInfomation.mbIsFullScreen]);
	}
	break;
	case ETitleVideoSettingButtonType::Apply:
	{
		// 비디오 정보 저장
		IFQGameInstanceInterface* MyGameInstance = Cast<IFQGameInstanceInterface>(GetWorld()->GetGameInstance());
		if (MyGameInstance)
		{
			MyGameInstance->SetVideoInfomation(mCurrentVideoInfomation);
		}

		// 메인 설정 창으로 돌아가기
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::MainSetting);
	}
	break;
	}
}

void UFQTitleVideoSettingUI::CancelButton()
{
	SettingUIDelegate.ExecuteIfBound(ESettingUIType::MainSetting);
}

void UFQTitleVideoSettingUI::PlayAnimation(float DeltaTime)
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

void UFQTitleVideoSettingUI::InitSelect()
{
	// UI 초기화
	mCurrentSelectIndex = ETitleVideoSettingButtonType::Resolution;
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

	// 비디오 정보 저장
	IFQGameInstanceInterface* MyGameInstance = Cast<IFQGameInstanceInterface>(GetWorld()->GetGameInstance());
	if (MyGameInstance)
	{
		mCurrentVideoInfomation = MyGameInstance->GetVideoInfomation();
	}

	mFullScreenCheck->SetBrushFromTexture(mCheckBoxs[mCurrentVideoInfomation.mbIsFullScreen]);
	mWindowScreenCheck->SetBrushFromTexture(mCheckBoxs[!mCurrentVideoInfomation.mbIsFullScreen]);
}

