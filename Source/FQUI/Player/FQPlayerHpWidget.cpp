#include "FQPlayerHpWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

UFQPlayerHpWidget::UFQPlayerHpWidget()
	: mHpPercent(1.f)
	, mHpDecrasePercent(1.f)
{
}

void UFQPlayerHpWidget::NativeConstruct()
{
	if (!mHpBar || !mHp || !mHpDecrase)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHpBar, mHp, mHpDecrase Image 가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	// 현재 Hp 이미지 가로 사이즈 값 초기화 적용
	mHpBar->SetVisibility(ESlateVisibility::Hidden);
	mHp->SetVisibility(ESlateVisibility::Hidden);
	mHpDecrase->SetVisibility(ESlateVisibility::Hidden);
}

void UFQPlayerHpWidget::UpdateHpDecrasePercent(float InDeltaTime)
{
	if (!mHpDecrase || FMath::Abs(mHpDecrasePercent - mHpPercent) < 0.0001f)
	{
		return;
	}

	if (mHpDecrasePercent > mHpPercent)
	{
		mHpDecrasePercent -= InDeltaTime * 0.1f;
	}
	else
	{
		mHpDecrasePercent = mHpPercent;
	}
	mHpDecrase->SetPercent(mHpDecrasePercent);
}

void UFQPlayerHpWidget::UpdatePlayerHp(float HpValue)
{
	if (!mHp)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mHp Or mHpDecrace 프로그래시브 바가 유효하지 않습니다!"), __LINE__);
		return;
	}

	mHpPercent = HpValue;
	mHp->SetPercent(mHpPercent);
}

void UFQPlayerHpWidget::UpdatePlayerControllerNumber(int32 PlayerControllerNumber, ESoulType SoulType)
{
	if (!mPlayerNumber)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mPlayerNumber Image Is Null!!"), __LINE__);
		return;
	}

	// 플레이어 넘버 세팅
	TSoftObjectPtr<UTexture2D>* TexturePtr = nullptr;
	switch (PlayerControllerNumber)
	{
	case 0:
		TexturePtr = mPlayerMap1.Find(SoulType);
		break;
	case 1:
		TexturePtr = mPlayerMap2.Find(SoulType);
		break;
	case 2:
		TexturePtr = mPlayerMap3.Find(SoulType);
		break;
	case 3:
		TexturePtr = mPlayerMap4.Find(SoulType);
		break;
	}

	if (TexturePtr)
	{
		UTexture2D* LoadedTexture = TexturePtr->LoadSynchronous();
		if (LoadedTexture)
		{
			mPlayerNumber->SetBrushFromTexture(LoadedTexture);
			UE_LOG(LogTemp, Log, TEXT("[UFQPlayerHpWidget %d] mPlayerNumber Image Changed Successfully"), __LINE__);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] Failed to Load Texture for SoulType: %d"), __LINE__, (int32)SoulType);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] TexturePtr is invalid or not found for SoulType: %d"), __LINE__, (int32)SoulType);
	}
}

void UFQPlayerHpWidget::SetHpBarVisibility(bool bIsVisible)
{
	if (!mHpBar || !mHp || !mHpDecrase)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHpBar, mHp, mHpDecrase 가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	if (bIsVisible)
	{
		mHpBar->SetVisibility(ESlateVisibility::Visible);
		mHp->SetVisibility(ESlateVisibility::Visible);
		mHpDecrase->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		mHpBar->SetVisibility(ESlateVisibility::Hidden);
		mHp->SetVisibility(ESlateVisibility::Hidden);
		mHpDecrase->SetVisibility(ESlateVisibility::Hidden);
	}
}
