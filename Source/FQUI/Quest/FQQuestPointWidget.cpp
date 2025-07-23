#include "FQQuestPointWidget.h"

#include "FQGameCore\Quest\FQQuestSystem.h"
#include "FQGameCore/GameInstance/FQGameInstanceInterface.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"

UFQQuestPointWidget::UFQQuestPointWidget()
	: mQuestID()
	, mQuestSignType(EQuestSignType::Boss)
	, mbIsQuestActive(false)
{
}

void UFQQuestPointWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UFQQuestPointWidget::UpdateQuestPosition(FVector2D DirectionDistance)
{
	if (!GEngine || !GEngine->GameViewport || !mSignBox)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestPointWidget %d] GEngine Or GameViewport Or mSignBox가 유효하지 않습니다!! "), __LINE__);
		return;
	}

	// 가까우면 UI 종료
	if (DirectionDistance.Length() < 500.f)
	{
		mSignBox->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	else
	{
		mSignBox->SetVisibility(ESlateVisibility::Visible);
	}

	// 거리 텍스트 수정
	FString DistanceText = FString::FromInt(DirectionDistance.Length() / 100) + TEXT("m");
	mDistanceText->SetText(FText::FromString(DistanceText));

	// 방향 벡터 정규화 (안정성 보장)
	if (!DirectionDistance.Normalize())
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestPointWidget %d] NormalDirection 정규화가 되지 않았습니다!!"), __LINE__);
		return;
	}

	// 화면 크기 가져오기
	FVector2D ViewportSize = (FVector2D(1920.f, 1000.f) - 100.f) * 0.5f;
	float ScreenRatio = ViewportSize.Length() / (ViewportSize.X + ViewportSize.Y);

	// 스냅 처리
	if (FMath::Abs(DirectionDistance.X) >= ScreenRatio)
	{
		DirectionDistance.X = DirectionDistance.X > 0 ? 1.f : -1.f;
	}
	if (FMath::Abs(DirectionDistance.Y) >= ScreenRatio)
	{
		DirectionDistance.Y = DirectionDistance.Y > 0 ? 1.f : -1.f;
	}

	// 최종 위치
	FVector2D FinalScreenPosition = FVector2D(DirectionDistance.Y * ViewportSize.X, -DirectionDistance.X * ViewportSize.Y );

	// VerticalBox의 Slot이 CanvasPanelSlot이면 위치 설정
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mSignBox->Slot))
	{
		// 필요 시 Anchor 또는 Alignment도 조정 가능
		CanvasSlot->SetPosition(FinalScreenPosition);
	}
}

void UFQQuestPointWidget::UpdateQuestActive(bool bIsActive)
{
	if (bIsActive)
	{
		if (auto FoundIcon = mQuestActiveSignTexture.Find(mQuestSignType))
		{
			mSign->SetBrushFromTexture(*FoundIcon);
		}
	}
	else
	{
		if (auto FoundIcon = mQuestInactiveSignTexture.Find(mQuestSignType))
		{
			mSign->SetBrushFromTexture(*FoundIcon);
		}
	}
}
