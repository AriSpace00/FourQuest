#include "FQInteractionQuest.h"

#include "FQGameCore\Quest/FQQuestSystem.h"
#include "FQGameCore/GameInstance/FQGameInstanceInterface.h"

AFQInteractionQuest::AFQInteractionQuest()
    : mInteractionType(EQuestInteractionType::None)
{
}

void AFQInteractionQuest::BeginPlay()
{
	// 퀘스트 시스템 델리게이트에 함수 등록
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		QuestSystem->mInteractionDelegate.AddUObject(this, &AFQInteractionQuest::TryUpdateQuestState);
	}

	// 텔레포트 타입이라면 현재 플레이 하고 있는 로컬 플레이어 수로 클리어 조건 맞추기
	if (mInteractionType == EQuestInteractionType::Teleport)
	{
		IFQGameInstanceInterface* MyGameInstance = Cast<IFQGameInstanceInterface>(GetGameInstance());
		if (MyGameInstance)
		{
			mQuestClearConditionNumber = MyGameInstance->GetPlayerCount();
		}
	}

	Super::BeginPlay();
}

void AFQInteractionQuest::TryUpdateQuestState(EQuestInteractionType InteractionType, int32 QuestConditionCount)
{
	// 퀘스트 시스템
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		// 현재 클리어 상태이면 종료
		FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(GetQuestID());
		if (MyQuestData->mbIsQuestClear)
		{
			return;
		}

		// 서브 퀘스트 클리어 여부 확인
		if (!MyQuestData->mbIsActive)
		{
			return;
		}
	}

	// 클리어 조건 갱신
    if (mInteractionType == InteractionType)
    {
		UpdateQuestCondition(QuestConditionCount);
    }
}


void AFQInteractionQuest::Execute()
{


}

void AFQInteractionQuest::UpdateQuest(float DeltaTime)
{
	Super::UpdateQuest(DeltaTime);


}