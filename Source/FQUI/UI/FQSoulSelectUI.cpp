// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulSelectUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Containers/UnrealString.h"

UFQSoulSelectUI::UFQSoulSelectUI()
	: mCurrentSoulType(ESoulType::Sword)
{
}

void UFQSoulSelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	mSoulNames.Add(ESoulType::Sword, FText::FromString(TEXT("루카스")));
	mSoulNames.Add(ESoulType::Staff, FText::FromString(TEXT("아르카나")));
	mSoulNames.Add(ESoulType::Axe, FText::FromString(TEXT("발더")));
	mSoulNames.Add(ESoulType::Bow, FText::FromString(TEXT("실버")));
	mSoulInfomations.Add(ESoulType::Sword, FText::FromString(TEXT("숙련 무기 : 검	\n\n 영혼공격 :\n 영혼의 검을 소환하여 휘두릅니다.\n 발산한 검기는 전방의 적을 강하게\n 밀치며 피해를 입힙니다.")));
	mSoulInfomations.Add(ESoulType::Staff, FText::FromString(TEXT("숙련 무기 : 검	\n\n 영혼공격 :\n 영혼의 완드를 소환하여 마법을 시전\n 합니다.\n 자신의 주변에 바람 장막을 형성하여\n 다가오는 적을 밀쳐냅니다.")));
	mSoulInfomations.Add(ESoulType::Axe, FText::FromString(TEXT("숙련 무기 : 워해머\n\n 영혼공격 :\n 영혼의 해머를 소환하여 휘두릅니다.\n 소용돌이치는 해머의 움직임으로 적\n 사이를 돌파합니다.")));
	mSoulInfomations.Add(ESoulType::Bow, FText::FromString(TEXT("숙련 무기 : 활\n\n 영혼공격 :\n 영혼의 활을 소환하여 화살을 난사합\n 니다.\n 엄청난 속도와 무차별 사격을 가합니다.")));
}

void UFQSoulSelectUI::UpdateSoulAnimation(int32 AnimationIndex)
{
	switch (mCurrentSoulType)
	{
	case ESoulType::Sword:
	{
		if (mSwordSoulAnimations.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectUI %d] mSoulAnimations Is Null!!"), __LINE__);
			return;
		}
		int32 AnimationKey = AnimationIndex % mSwordSoulAnimations.Num();
		mSoulAnimation->SetBrushFromTexture(mSwordSoulAnimations[AnimationKey]);
		mSoulName->SetText(mSoulNames[mCurrentSoulType]);
		mSoulInfomation->SetText(mSoulInfomations[mCurrentSoulType]);
	}
	break;
	case ESoulType::Staff:
	{
		if (mStaffSoulAnimations.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectUI %d] mSoulAnimations Is Null!!"), __LINE__);
			return;
		}
		int32 AnimationKey = AnimationIndex % mStaffSoulAnimations.Num();
		mSoulAnimation->SetBrushFromTexture(mStaffSoulAnimations[AnimationKey]);
		mSoulName->SetText(mSoulNames[mCurrentSoulType]);
		mSoulInfomation->SetText(mSoulInfomations[mCurrentSoulType]);
	}
	break;
	case ESoulType::Axe:
	{
		if (mAxeSoulAnimations.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectUI %d] mSoulAnimations Is Null!!"), __LINE__);
			return;
		}
		int32 AnimationKey = AnimationIndex % mAxeSoulAnimations.Num();
		mSoulAnimation->SetBrushFromTexture(mAxeSoulAnimations[AnimationKey]);
		mSoulName->SetText(mSoulNames[mCurrentSoulType]);
		mSoulInfomation->SetText(mSoulInfomations[mCurrentSoulType]);
	}
	break;
	case ESoulType::Bow:
	{
		if (mBowSoulAnimations.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectUI %d] mSoulAnimations Is Null!!"), __LINE__);
			return;
		}
		int32 AnimationKey = AnimationIndex % mBowSoulAnimations.Num();
		mSoulAnimation->SetBrushFromTexture(mBowSoulAnimations[AnimationKey]);
		mSoulName->SetText(mSoulNames[mCurrentSoulType]);
		mSoulInfomation->SetText(mSoulInfomations[mCurrentSoulType]);
	}
	break;
	}
}

void UFQSoulSelectUI::UpdateSoulType(ESoulType NewSoulType)
{
	mCurrentSoulType = NewSoulType;
	if (!mEmptyIcon || !mCheckIcon)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQSoulSelectUI %d] mEmptyIcon Or mCheckIcon Texture Is Null!!"), __LINE__);
		return;
	}

	switch (NewSoulType)
	{
	case ESoulType::Sword:
	{
		mSoulIndex1->SetBrushFromTexture(mCheckIcon);
		mSoulIndex2->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex3->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex4->SetBrushFromTexture(mEmptyIcon);
	}
	break;
	case ESoulType::Staff:
	{
		mSoulIndex1->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex2->SetBrushFromTexture(mCheckIcon);
		mSoulIndex3->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex4->SetBrushFromTexture(mEmptyIcon);
	}
	break;
	case ESoulType::Axe:
	{
		mSoulIndex1->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex2->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex3->SetBrushFromTexture(mCheckIcon);
		mSoulIndex4->SetBrushFromTexture(mEmptyIcon);
	}
	break;
	case ESoulType::Bow:
	{
		mSoulIndex1->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex2->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex3->SetBrushFromTexture(mEmptyIcon);
		mSoulIndex4->SetBrushFromTexture(mCheckIcon);
	}
	break;
	}
}