#include "FQTitleSettingUI.h"

#include "Components\Image.h"
#include "Components\CanvasPanelSlot.h"
#include "Components/TextBlock.h"


UFQTitleSettingUI::UFQTitleSettingUI()
	: mCurrentSelectIndex(ETitleSettingButtonType::Video)
	, mElapsedTime()
	, mCurrentFrameIndex()
{
	mGuides.Add(ETitleSettingButtonType::Video, FText::FromString(TEXT("해상도 및 화면 환경을 설정합니다.")));
	mGuides.Add(ETitleSettingButtonType::Audio, FText::FromString(TEXT("오디오 음량을 조절할 수 있습니다.")));
	mGuides.Add(ETitleSettingButtonType::Back, FText::FromString(TEXT("게임으로 돌아갑니다.")));
}

void UFQTitleSettingUI::NativeConstruct()
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
	{
		mOriginYPos = CanvasSlot->GetPosition().Y;
	}
}

void UFQTitleSettingUI::PlayAnimation(float DeltaTime)
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

void UFQTitleSettingUI::InitSelect()
{
	mCurrentSelectIndex = ETitleSettingButtonType::Video;
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}
}

void UFQTitleSettingUI::Move(EWidgetInputType InputType)
{
	if (mSelectButton == nullptr || mSelectAnimation == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Is nullptr!!"), __LINE__);
		return;
	}

	if (InputType == EWidgetInputType::Up && mCurrentSelectIndex != ETitleSettingButtonType::Video)
	{
		mCurrentSelectIndex = (ETitleSettingButtonType)((uint8)(mCurrentSelectIndex)-1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -170));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -170));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentSelectIndex != ETitleSettingButtonType::Back)
	{
		mCurrentSelectIndex = (ETitleSettingButtonType)((uint8)mCurrentSelectIndex + 1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 170));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 170));
		}
	}

	// 설명 텍스트 세팅
	mTextGuide->SetText(mGuides[mCurrentSelectIndex]);
}

void UFQTitleSettingUI::SelectButton()
{
	switch (mCurrentSelectIndex)
	{
	case ETitleSettingButtonType::Video:
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::VideoSetting);
		break;
	case ETitleSettingButtonType::Audio:
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::AudioSetting);
		break;
	case ETitleSettingButtonType::Back:
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::None);
		break;
	}
}

void UFQTitleSettingUI::CancelButton()
{
	SettingUIDelegate.ExecuteIfBound(ESettingUIType::None);
}
