#include "FQInGameSettingWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Components/Image.h"
#include "Components\CanvasPanelSlot.h"
#include "Components/TextBlock.h"

#include "FQGameCore\GameInstance\FQGameInstanceInterface.h"
#include "FQGameCore\GameMode\FQGameModeUIInputInterface.h"

UFQInGameSettingWidget::UFQInGameSettingWidget()
	: mElapsedTime()
	, mCurrentFrameIndex()
	, mCurrentButtonType(EGameSettingButtonType::Continue)
{
	mGuides.Add(EGameSettingButtonType::Continue, FText::FromString(TEXT("게임으로 다시 돌아갑니다.")));
	mGuides.Add(EGameSettingButtonType::Setting, FText::FromString(TEXT("게임 내 환경을 설정합니다.")));
	mGuides.Add(EGameSettingButtonType::Failed, FText::FromString(TEXT("미션을 중단하고 타이틀 화면으로 돌아갑니다.")));
	mGuides.Add(EGameSettingButtonType::Logout, FText::FromString(TEXT("게임을 종료합니다.")));
}

void UFQInGameSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
	{
		mOriginYPos = CanvasSlot->GetPosition().Y;
	}
}

void UFQInGameSettingWidget::InitSelect()
{
	// UI 초기화
	mCurrentButtonType = EGameSettingButtonType::Continue;
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X, mOriginYPos));
	}

	mGuideText->SetText(mGuides[mCurrentButtonType]);
}

void UFQInGameSettingWidget::Move(EWidgetInputType InputType)
{
	if (mSelectButton == nullptr || mSelectAnimation == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQInGameSettingWidget %d] mSelectButton Or mSelectAnimation Is nullptr!!"), __LINE__);
		return;
	}

	if (InputType == EWidgetInputType::Up && mCurrentButtonType != EGameSettingButtonType::Continue)
	{
		mCurrentButtonType = (EGameSettingButtonType)((uint8)(mCurrentButtonType)-1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -120));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -120));
		}
	}
	else if (InputType == EWidgetInputType::Down && mCurrentButtonType != EGameSettingButtonType::Logout)
	{
		mCurrentButtonType = (EGameSettingButtonType)((uint8)mCurrentButtonType + 1);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 120));
		}
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectAnimation->Slot))
		{
			CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 120));
		}
	}

	if (mGuideText)
	{
		mGuideText->SetText(mGuides[mCurrentButtonType]);
	}
}

void UFQInGameSettingWidget::SelectButton()
{
	IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
	if (!MyGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQInGameSettingWidget %d] IFQGameInstanceInterface가 유효하지 않습니다!!"), __LINE__);
		return;
	}
	
	switch (mCurrentButtonType)
	{
	case EGameSettingButtonType::Continue:
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::End);
		break;
	case EGameSettingButtonType::Setting:
		SettingUIDelegate.ExecuteIfBound(ESettingUIType::MainSetting);
		break;
	case EGameSettingButtonType::Failed:
		MyGameMode->StartGame();
		break;
	case EGameSettingButtonType::Logout:
		MyGameMode->ExitGame();
		break;
	}
}

void UFQInGameSettingWidget::CancelButton()
{
	SettingUIDelegate.ExecuteIfBound(ESettingUIType::End);
}

void UFQInGameSettingWidget::PlayAnimation(float DeltaTime)
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