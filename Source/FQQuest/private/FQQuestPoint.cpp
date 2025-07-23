#include "FQQuestPoint.h"

#include "FQGameCore\Quest\FQQuestSystem.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"

#include "GameFramework/GameModeBase.h"

AFQQuestPoint::AFQQuestPoint()
    : mQuestID(1)
    , mbIsQuestActive(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestPoint::BeginPlay()
{
	Super::BeginPlay();
	
    // UI 생성
    if (mQuestPointWidgetClass)
    {
        mQuestPointWidget = CreateWidget<UFQQuestPointWidget>(GetWorld(), mQuestPointWidgetClass);
        if (mQuestPointWidget)
        {
            mQuestPointWidget->AddToViewport(10); // 화면에 위젯 추가
            mQuestPointWidget->SetQuestSignType(mQuestSignType);
            mQuestPointWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 콜백 함수 등록
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        QuestSystem->mQuestActiveDelegate.AddUObject(this, &AFQQuestPoint::UpdateQuestActive);
        FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(mQuestID);
        if (MyQuestData)
        {
            mbIsQuestActive = MyQuestData->mbIsActive;
        }
    }
}

void AFQQuestPoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (mQuestPointWidget && mQuestPointWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        IFQGameModeInterface* MyGameMode = Cast<IFQGameModeInterface>(GetWorld()->GetAuthGameMode());
        if (MyGameMode)
        {
            FVector MyLocation = GetTransform().GetLocation();
            FVector CameraLocation = MyGameMode->GetMainCameraTransform().GetLocation();

            // 방향 벡터 계산 (Z축 무시)
            FVector Direction3D = (MyLocation - CameraLocation);
            Direction3D.Z = 0; // 수평 방향만 고려

            // 2D 방향 벡터로 변환
            FVector2D Direction2D(Direction3D.X, Direction3D.Y);

            mQuestPointWidget->UpdateQuestPosition(Direction2D);
        }
    }
}

void AFQQuestPoint::SetVisible(bool IsVisible)
{
    if (!mQuestPointWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQQuestPoint %d] mQuestPointWidget가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    if (IsVisible)
    {
        mQuestPointWidget->SetVisibility(ESlateVisibility::Visible);
        mQuestPointWidget->UpdateQuestActive(mbIsQuestActive);
    }
    else
    {
        mQuestPointWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AFQQuestPoint::UpdateQuestActive(int32 QuestID, bool bIsQuestActive)
{
    if (mQuestPointWidget && mQuestID == QuestID)
    {
        mbIsQuestActive = bIsQuestActive;
        mQuestPointWidget->UpdateQuestActive(bIsQuestActive);
    }
}

