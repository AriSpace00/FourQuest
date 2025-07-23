// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulSelectScreenWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "FQSoulComplateUI.h"
#include "FQSoulSelectUI.h"

UFQSoulSelectScreenWidget::UFQSoulSelectScreenWidget()
    : mCurrentFrameIndex()
    , mAnimationElapsedTime()
    , mCurrentReadyCount()
    , mAllReady()
    , mReadyElapsedTime()
    , mStartNextLevel(false)
{
}

void UFQSoulSelectScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

    constexpr int32 NumWidgets = 4; // 원하는 개수만큼 반복
    for (int32 i = 1; i <= NumWidgets; ++i)
    {
        // NoSelect Image
        FString NoSelectName = FString::Printf(TEXT("P%d_NoSelect"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*NoSelectName)))
        {
            if (UImage* Img = Cast<UImage>(FoundWidget))
            {
                FoundWidget->SetVisibility(ESlateVisibility::Visible);
                mNoSelectSoulArr.Add(Img);
                UE_LOG(LogTemp, Log, TEXT("[UFQSoulSelectScreenWidget %d] UImage(%s) Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] UImage(%s) Can't Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] NoSelectName(%s) Can't Find It!! : %d"), __LINE__,*NoSelectName, i);
        }

        // Soul Select UI
        FString SelectUIName = FString::Printf(TEXT("P%d_SelectCheck"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*SelectUIName)))
        {
            if (UFQSoulSelectUI* SelectUI = Cast<UFQSoulSelectUI>(FoundWidget))
            {
                SelectUI->SetVisibility(ESlateVisibility::Hidden);
                SelectUI->SetOwningActor(mOwningActor);
                mSoulSelectUIArr.Add(SelectUI);
                UE_LOG(LogTemp, Log, TEXT("[UFQSoulSelectScreenWidget %d] UFQSoulSelectUI(%s) Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] UFQSoulSelectUI(%s) Can't Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] SelectUIName(%s) Can't Find It!! : %d"), __LINE__, *SelectUIName, i);
        }

        // Soul Complete UI
        FString ComplateUIName = FString::Printf(TEXT("P%d_ComplateSoul"), i);
        if (UWidget* FoundWidget = GetWidgetFromName(FName(*ComplateUIName)))
        {
            if (UFQSoulComplateUI* ComplateUI = Cast<UFQSoulComplateUI>(FoundWidget))
            {
                ComplateUI->SetVisibility(ESlateVisibility::Hidden);
                ComplateUI->SetOwningActor(mOwningActor);
                mSoulComplateUIArr.Add(ComplateUI);
                UE_LOG(LogTemp, Log, TEXT("[UFQSoulSelectScreenWidget %d] UFQSoulComplateUI(%s) Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] UFQSoulComplateUI(%s) Can't Find It!! : %d"), __LINE__, *NoSelectName, i);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectScreenWidget %d] ComplateUIName(%s) Can't Find It!! : %d"), __LINE__, *ComplateUIName, i);
        }
    }

    mReadyBackGround->SetVisibility(ESlateVisibility::Hidden);
    mReadyCount->SetVisibility(ESlateVisibility::Hidden);
    mReadyText->SetVisibility(ESlateVisibility::Hidden);
}

void UFQSoulSelectScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    mAnimationElapsedTime += InDeltaTime;
    if (mAnimationElapsedTime > 0.1f)
    {
        mAnimationElapsedTime = 0.f;

        // 애니메이션 플레이
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % 50;
        for (auto& SoulSelectUI : mSoulSelectUIArr)
        {
			SoulSelectUI->UpdateSoulAnimation(mCurrentFrameIndex);
        }
    }

    if (mAllReady)
    {
        mReadyElapsedTime += InDeltaTime;
        if (mReadyElapsedTime > 1.f)
        {
            mReadyElapsedTime = 0.f;
            mCurrentReadyCount++;

            if (mCurrentReadyCount == mReadyCountText.Num())
            {
                mStartNextLevel = true;
            }
            else
            {
                mReadyCount->SetText(mReadyCountText[mCurrentReadyCount]);
            }
        }
    }
    else
    {
        mCurrentReadyCount = 0;
        mReadyElapsedTime = 0.f;
        mReadyCount->SetText(mReadyCountText[mCurrentReadyCount]);
    }
}

void UFQSoulSelectScreenWidget::WidgetInput(ESoulType SoulType, int32 ControllerId)
{
	MoveIndex(SoulType, ControllerId);
}

void UFQSoulSelectScreenWidget::UpdatePlayerState(EPlayerStateType PlayerStateType, ESoulType SoulType, int32 ControllerId)
{
	switch (PlayerStateType)
	{
	case EPlayerStateType::NoSelect:
	{
		mNoSelectSoulArr[ControllerId]->SetVisibility(ESlateVisibility::Visible);
		mSoulSelectUIArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);
		mSoulComplateUIArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);

        mAllReady = true;
        bool bCheckReady = false;
        for (int32 i = 0; i < 4; i++)
        {
            if (mSoulComplateUIArr[i]->GetVisibility() == ESlateVisibility::Visible)
            {
                bCheckReady = true;
            }
            if (mSoulSelectUIArr[i]->GetVisibility() == ESlateVisibility::Visible)
            {
                mAllReady = false;
            }
        }

        if (mAllReady)
        {
            mAllReady = bCheckReady;
        }

        if (mAllReady)
        {
            mReadyBackGround->SetVisibility(ESlateVisibility::Visible);
            mReadyCount->SetVisibility(ESlateVisibility::Visible);
            mReadyText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            mReadyBackGround->SetVisibility(ESlateVisibility::Hidden);
            mReadyCount->SetVisibility(ESlateVisibility::Hidden);
            mReadyText->SetVisibility(ESlateVisibility::Hidden);
        }
	}
	break;
	case EPlayerStateType::SoulSelect:
	{
		mNoSelectSoulArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);
		mSoulSelectUIArr[ControllerId]->SetVisibility(ESlateVisibility::Visible);
		mSoulComplateUIArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);

        mAllReady = false;
        mReadyBackGround->SetVisibility(ESlateVisibility::Hidden);
        mReadyCount->SetVisibility(ESlateVisibility::Hidden);
        mReadyText->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case EPlayerStateType::SoulComplate:
	{
		mNoSelectSoulArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);
		mSoulSelectUIArr[ControllerId]->SetVisibility(ESlateVisibility::Hidden);
		mSoulComplateUIArr[ControllerId]->SetVisibility(ESlateVisibility::Visible);

        mSoulComplateUIArr[ControllerId]->UpdateSoulAnimation(SoulType);

        mAllReady = true;
        for (int32 i = 0; i < 4; i++)
        {
            if (mSoulSelectUIArr[i]->GetVisibility() == ESlateVisibility::Visible)
            {
                mAllReady = false;
            }
        }

        if (mAllReady)
        {
            mReadyBackGround->SetVisibility(ESlateVisibility::Visible);
            mReadyCount->SetVisibility(ESlateVisibility::Visible);
            mReadyText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            mReadyBackGround->SetVisibility(ESlateVisibility::Hidden);
            mReadyCount->SetVisibility(ESlateVisibility::Hidden);
            mReadyText->SetVisibility(ESlateVisibility::Hidden);
        }
	}
	break;
	}
}

void UFQSoulSelectScreenWidget::MoveIndex(ESoulType SoulType, int32 ControllerId)
{
    if (mSoulSelectUIArr[ControllerId]->GetVisibility() == ESlateVisibility::Visible)
    {
        mSoulSelectUIArr[ControllerId]->UpdateSoulType(SoulType);
    }
}