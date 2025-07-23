#include "FQMonsterHpWidget.h"

#include "Components/ProgressBar.h"

UFQMonsterHpWidget::UFQMonsterHpWidget()
	: mHpPercent(1.f)
	, mHpDecrasePercent(1.f)
	, mHpDecraseSpeed(0.2f)
{
}

void UFQMonsterHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!mHp || !mHpDecrase)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHpWidget %d] mHp, mHpDecrase Image 가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	mHpDecrase->SetPercent(1.f);
	mHp->SetPercent(1.f);
}

void UFQMonsterHpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!mHpDecrase || FMath::Abs(mHpDecrasePercent - mHpPercent) < 0.0001f)
	{
		return;
	}

	if (mHpDecrasePercent > mHpPercent)
	{
		mHpDecrasePercent -= InDeltaTime * mHpDecraseSpeed;
	}
	else
	{
		mHpDecrasePercent = mHpPercent;
	}
	mHpDecrase->SetPercent(mHpDecrasePercent);
}

void UFQMonsterHpWidget::UpdateHp(float HpValue)
{
	if (!mHp)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQPlayerHUDWidget %d] mHp Or mHpDecrace 프로그래시브 바가 유효하지 않습니다!"), __LINE__);
		return;
	}

	mHpPercent = HpValue;
	mHp->SetPercent(mHpPercent);
}
