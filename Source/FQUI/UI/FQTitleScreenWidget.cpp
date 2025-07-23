// Fill out your copyright notice in the Description page of Project Settings.


#include "FQTitleScreenWidget.h"
#include "FQGameCore\GameMode/FQGameModeUIInputInterface.h"
#include "FQTitleSettingUI.h"
#include "FQTitleAudioSettingUI.h"
#include "FQTitleVideoSettingUI.h"

#include "Components\Image.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/GameModeBase.h"


UFQTitleScreenWidget::UFQTitleScreenWidget()
    : mElapsedTime(0.f)
    , mCurrentFrameIndex(0)
    , mCurrentSelectIndex(ETitleButtonType::GameStart)
    , mCurrentSettingType(ESettingUIType::None)
    , mSoulAnimations{}
    , mSelectButton()
    , mCurrentSelect()
{
}

void UFQTitleScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Image Is Null!!"), __LINE__);
        return;
    }

    mTitleSettingUI->SettingUIDelegate.BindUObject(this, &UFQTitleScreenWidget::TitleSetting);
    mTitleVideoSettingUI->SettingUIDelegate.BindUObject(this, &UFQTitleScreenWidget::TitleSetting);
    mTitleAudioSettingUI->SettingUIDelegate.BindUObject(this, &UFQTitleScreenWidget::TitleSetting);

    TitleSetting(ESettingUIType::None);
}

void UFQTitleScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    switch (mCurrentSettingType)
    {
    case ESettingUIType::MainSetting:
        mTitleSettingUI->PlayAnimation(InDeltaTime);
        break;
    case ESettingUIType::VideoSetting:
        mTitleVideoSettingUI->PlayAnimation(InDeltaTime);
        break;
    case ESettingUIType::AudioSetting:
        mTitleAudioSettingUI->PlayAnimation(InDeltaTime);
        break;
    }

    mElapsedTime += InDeltaTime;
    if (mElapsedTime < 0.1f)
    {
        return;
    }

    // 애니메이션 플레이
    mElapsedTime = 0.f;
    if (mSoulAnimations.Num() > 0)
    {
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mSoulAnimations.Num();
        if (mCurrentSelect)
        {
            mCurrentSelect->SetBrushFromTexture(mSoulAnimations[mCurrentFrameIndex]);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mCurrentSelect Is Nullptr!!"), __LINE__);
        }
    }
}

void UFQTitleScreenWidget::WidgetInput(EWidgetInputType InputType)
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

void UFQTitleScreenWidget::TitleSetting(ESettingUIType SettingType)
{
    if (!mTitleSettingUI || !mTitleVideoSettingUI || !mTitleAudioSettingUI)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mTitleSettingUI, mTitleVideoSettingUI, mTitleAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    mCurrentSettingType = SettingType;
    switch (mCurrentSettingType)
    {
	case ESettingUIType::None:
    {
        mTitleSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
    }
	break;
	case ESettingUIType::MainSetting:
	{
        mTitleSettingUI->SetVisibility(ESlateVisibility::Visible);
        mTitleVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case ESettingUIType::VideoSetting:
	{
        mTitleVideoSettingUI->InitSelect();

        mTitleSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleVideoSettingUI->SetVisibility(ESlateVisibility::Visible);
        mTitleAudioSettingUI->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case ESettingUIType::AudioSetting:
	{
        mTitleAudioSettingUI->InitSelect();

        mTitleSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleVideoSettingUI->SetVisibility(ESlateVisibility::Hidden);
        mTitleAudioSettingUI->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	}
}

void UFQTitleScreenWidget::MoveIndex(EWidgetInputType InputType)
{
    if (!mTitleSettingUI || !mTitleVideoSettingUI || !mTitleAudioSettingUI)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mTitleSettingUI, mTitleVideoSettingUI, mTitleAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    switch (mCurrentSettingType)
    {
    case ESettingUIType::None:
        Move(InputType);
        break;
    case ESettingUIType::MainSetting:
        mTitleSettingUI->Move(InputType);
        break;
    case ESettingUIType::VideoSetting:
        mTitleVideoSettingUI->Move(InputType);
        break;
    case ESettingUIType::AudioSetting:
        mTitleAudioSettingUI->Move(InputType);
        break;
    }
}

void UFQTitleScreenWidget::SelectButton()
{
    if (!mTitleSettingUI || !mTitleVideoSettingUI || !mTitleAudioSettingUI)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mTitleSettingUI, mTitleVideoSettingUI, mTitleAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    switch (mCurrentSettingType)
    {
    case ESettingUIType::None:
        TitleSelectButton();
        break;
    case ESettingUIType::MainSetting:
        mTitleSettingUI->SelectButton();
        break;
    case ESettingUIType::VideoSetting:
        mTitleVideoSettingUI->SelectButton();
        break;
    case ESettingUIType::AudioSetting:
        mTitleAudioSettingUI->SelectButton();
        break;
    }
}

void UFQTitleScreenWidget::CancelButton()
{
    if (!mTitleSettingUI || !mTitleVideoSettingUI || !mTitleAudioSettingUI)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] mTitleSettingUI, mTitleVideoSettingUI, mTitleAudioSettingUI가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    switch (mCurrentSettingType)
    {
    case ESettingUIType::MainSetting:
        mTitleSettingUI->CancelButton();
        break;
    case ESettingUIType::VideoSetting:
        mTitleVideoSettingUI->CancelButton();
        break;
    case ESettingUIType::AudioSetting:
        mTitleAudioSettingUI->CancelButton();
        break;
    }
}

void UFQTitleScreenWidget::Move(EWidgetInputType MoveType)
{
    if (mSelectButton == nullptr || mCurrentSelect == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQTitleWidget %d] mSelectButton Or mCurrentSelect Is nullptr!!"), __LINE__);
        return;
    }

    if (MoveType == EWidgetInputType::Up && mCurrentSelectIndex != ETitleButtonType::GameStart)
    {
        mCurrentSelectIndex = (ETitleButtonType)((uint8)(mCurrentSelectIndex)-1);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, -100));
        }
    }
    else if (MoveType == EWidgetInputType::Down && mCurrentSelectIndex != ETitleButtonType::GameExit)
    {
        mCurrentSelectIndex = (ETitleButtonType)((uint8)mCurrentSelectIndex + 1);

        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSelectButton->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 100));
        }
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mCurrentSelect->Slot))
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + FVector2D(0, 100));
        }
    }
}

void UFQTitleScreenWidget::TitleSelectButton()
{
    switch (mCurrentSelectIndex)
    {
    case ETitleButtonType::GameStart:
    {
        IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
        if (MyGameMode)
        {
            MyGameMode->StartGame();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] IFQGameModeInterface가 유효하지 않습니다!"), __LINE__);
		}
	}
	break;
	case ETitleButtonType::GameSetting:
	{
        TitleSetting(ESettingUIType::MainSetting);
	}
	break;
	case ETitleButtonType::GameExit:
	{
        IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
        if (MyGameMode)
        {
            MyGameMode->ExitGame();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQTitleScreenWidget %d] IFQGameModeInterface가 유효하지 않습니다!"), __LINE__);
        }
	}
	break;
	}
}
