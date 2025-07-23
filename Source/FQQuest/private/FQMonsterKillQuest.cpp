#include "FQMonsterKillQuest.h"

#include "FQGameCore\Quest/FQQuestSystem.h"

AFQMonsterKillQuest::AFQMonsterKillQuest()
    : mMonsterType(EQuestMonsterType::None)
{
}

void AFQMonsterKillQuest::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 등록
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		QuestSystem->mMonsterQuestDelegate.AddUObject(this, &AFQMonsterKillQuest::TryUpdateQuestState);
	}
}

void AFQMonsterKillQuest::TryUpdateQuestState(EQuestMonsterType MonsterType, FName MonsterGroupID)
{
	// 퀘스트 시스템
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		// 현재 클리어 상태이면 종료
		FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(GetQuestID());
		if (MyQuestData->mbIsQuestClear)
		{
			UE_LOG(LogTemp, Log, TEXT("[AFQMonsterKillQuest %d] 퀘스트가 클리어가 된 상황입니다!!"), __LINE__);
			return;
		}

		// 서브 퀘스트 클리어 여부 확인
		if (!MyQuestData->mbIsActive)
		{
			UE_LOG(LogTemp, Log, TEXT("[AFQMonsterKillQuest %d] 퀘스트가 활성화되지 않았습니다!!"), __LINE__);
			return;
		}

		// 퀘스트 조건 갱신
		if (mMonsterType == MonsterType)
		{
			if (mMonsterType == EQuestMonsterType::MonsterGroup && mMonsterGroupID == MonsterGroupID)
			{
				UpdateQuestCondition(1);
			}
			else
			{
				UpdateQuestCondition(1);
			}
		}

		// 부모 퀘스트 액티브
		if (mOwnerQuest.IsValid())
		{
			QuestSystem->GetQuestData(mOwnerQuest.Get()->GetQuestID());
		}
	}
}

void AFQMonsterKillQuest::Execute()
{
}

void AFQMonsterKillQuest::UpdateQuest(float DeltaTime)
{
	Super::UpdateQuest(DeltaTime);


}
