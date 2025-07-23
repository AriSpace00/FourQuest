#include "FQQuestManager.h"

#include "FQQuestTriggerVolume.h"
#include "FQMonsterKillQuest.h"
#include "FQInteractionQuest.h"
#include "FQGameCore\Quest\FQQuestSystem.h"
#include "FQUI/Quest/FQQuestListUI.h"
#include "FQUI/Quest/FQQuestWidget.h"
#include "FQQuestRewardDataAsset.h"
#include "FQQuestPoint.h"

#include "EngineUtils.h"
#include <Kismet\GameplayStatics.h>

AFQQuestManager::AFQQuestManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestManager::BeginPlay()
{
	Super::BeginPlay();

    // UI 생성
    if (mQuestListUIClass)
    {
        mQuestListUI = CreateWidget<UFQQuestListUI>(GetWorld(), mQuestListUIClass);
        if (mQuestListUI)
        {
            mQuestListUI->AddToViewport(); // 화면에 위젯 추가
            UE_LOG(LogTemp, Log, TEXT("[AFQQuestManager %d] Create HUD Widget And AddViewport"), __LINE__);
        }
    }
	
    // 콜백 함수 등록
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        QuestSystem->mQuestTriggerDelegate.BindUObject(this, &AFQQuestManager::OnTriggerCallbackFunction);
        QuestSystem->mQuestClearDelegate.AddUObject(this, &AFQQuestManager::ClearQuest);
    }
    
    // 첫 퀘스트 시작 번호 생성
    CreateQuest(mQuestStartID);
}

void AFQQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<int32> QuestKeys;
    mQuestList.GetKeys(QuestKeys);

    TArray<int32> QuestsToRemove;
    for (auto& QuestKey : QuestKeys)
    {
        if (!mQuestList.Contains(QuestKey)) continue;

        TObjectPtr<AFQQuestBase> Quest = mQuestList[QuestKey];
        if (!IsValid(Quest))
        {
            QuestsToRemove.Add(QuestKey);
            continue;
        }

        // 퀘스트 업데이트
        Quest->UpdateQuest(DeltaTime);

        // 퀘스트 삭제
        EQuestStateType MyStateType = Quest->GetCurrentState();
        if (MyStateType == EQuestStateType::End)
        {
            QuestsToRemove.Add(QuestKey);
        }
    }

    // 퀘스트 삭제
    for (int32 Key : QuestsToRemove)
    {
        // RemoveAndCopyValue는 제거 + 값 추출 (중복 액세스 방지)
        TObjectPtr<AFQQuestBase> Quest = nullptr;
        if (mQuestList.RemoveAndCopyValue(Key, Quest) && IsValid(Quest))
        {
            if (UFQQuestWidget* MyQuestWidget = Quest->GetQuestWidget())
            {
                if (IsValid(mQuestListUI))
                {
                    mQuestListUI->RemoveQuestWidget(MyQuestWidget);
                }
            }

            Quest->Destroy();
        }
    }
}

void AFQQuestManager::OnTriggerCallbackFunction(int32 QuestID, EQuestTriggerType QuestTrigeerType)
{
	switch (QuestTrigeerType)
	{
	case EQuestTriggerType::QuestStarts:
	{
        // 퀘스트 생성
		CreateQuest(QuestID);
	}
	break;
    case EQuestTriggerType::QuestClear:
	{
        auto MyQuest = mQuestList.Find(QuestID);
        if (MyQuest)
        {
            (*MyQuest)->SetNewState(EQuestStateType::Exit);
        }
	}
	break;
	}
}

void AFQQuestManager::CreateQuest(int32 QuestID)
{
    // 퀘스트 시스템 탐색
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        FFQQuestTable* QuestData = QuestSystem->GetQuestData(QuestID);
        if (!QuestData)
        {
            UE_LOG(LogTemp, Error, TEXT("[AFQQuestManager %d] QuestData가 유효하지 않습니다!!"), __LINE__);
            return;
        }

        // 퀘스트 생성
        switch (QuestData->QuestType)
        {
        case EQuestType::MonsterKill:
        {
            AFQMonsterKillQuest* MonsterKillQuest = GetWorld()->SpawnActorDeferred<AFQMonsterKillQuest>(AFQMonsterKillQuest::StaticClass(), FTransform());
            MonsterKillQuest->SetQuestID(QuestData->QuestNumber);
            MonsterKillQuest->SetQuestClearConditionNumber(QuestData->QuestClearConditionsNumber);
            MonsterKillQuest->SetQuestDescription(QuestData->QuestDescription);
            MonsterKillQuest->SetQuestMonsterType(QuestData->QuestMonsterType);
            MonsterKillQuest->SetMonsterGroupID(QuestData->QuestMonsterGroupName);
            MonsterKillQuest->FinishSpawning(FTransform());
            mQuestList.Emplace(QuestData->QuestNumber, *MonsterKillQuest);

            UFQQuestWidget* MyQuestWidget = MonsterKillQuest->GetQuestWidget();
            if (MyQuestWidget)
            {
                mQuestListUI->AddQuestListWidget(MyQuestWidget);
            }
        }
        break;
        case EQuestType::Interaction:
        {
            AFQInteractionQuest* InteractionQuest = GetWorld()->SpawnActorDeferred<AFQInteractionQuest>(AFQInteractionQuest::StaticClass(), FTransform());
            InteractionQuest->SetQuestID(QuestData->QuestNumber);
            InteractionQuest->SetQuestClearConditionNumber(QuestData->QuestClearConditionsNumber);
            InteractionQuest->SetQuestInteractionType(QuestData->QuestInteractionType);
            InteractionQuest->SetQuestDescription(QuestData->QuestDescription);
            InteractionQuest->FinishSpawning(FTransform());
            mQuestList.Emplace(QuestData->QuestNumber, *InteractionQuest);

            UFQQuestWidget* MyQuestWidget = InteractionQuest->GetQuestWidget();
            if (MyQuestWidget)
            {
                mQuestListUI->AddQuestListWidget(MyQuestWidget);
            }
        }
        break;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQQuestManager %d] QuestSystem가 유효하지 않습니다!!"), __LINE__);
    }
}

void AFQQuestManager::ClearQuest(int32 QuestID)
{
    // 퀘스트 시스템
    UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
    if (QuestSystem)
    {
        // 다음 퀘스트 생성
        const FFQQuestTable* CurrentQuestData = QuestSystem->GetQuestData(QuestID);
        if (CurrentQuestData)
        {
            for (const int32& NextQuestID : CurrentQuestData->NextQuestList)
            {
                if (QuestSystem->GetQuestData(NextQuestID)->mbIsQuestClear)
                {
                    continue;
                }

                CreateQuest(NextQuestID);
            }
        }
    }

    // 퀘스트 보상
    auto QuestReward = mQuestRewardList.Find(QuestID);
    if (QuestReward && *QuestReward)
    {
        FTransform SpwanTransform;
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFQQuestPoint::StaticClass(), FoundActors);
        for (auto Actor : FoundActors)
        {
            AFQQuestPoint* QuestPoint = Cast<AFQQuestPoint>(Actor);
            if (QuestPoint && QuestPoint->GetQuestID() == (*QuestReward)->mNextQuestID)
            {
                SpwanTransform = QuestPoint->GetTransform();
                break;
            }
        }

        // 액터 생성
        for (auto SpawnActor : (*QuestReward)->mRewordSpawnActor)
        {
            GetWorld()->SpawnActor<AActor>(SpawnActor, SpwanTransform);
        }

        // 다음 레벨 이동 여부
        if ((*QuestReward)->mbOpenNextLevel)
        {
            if ((*QuestReward)->mNextLevelName.IsNone())
            {
                UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_Title] TargetLevelName is not set!"));
                return;
            }

            UGameplayStatics::OpenLevel(this, (*QuestReward)->mNextLevelName);
            UE_LOG(LogTemp, Log, TEXT("[AFQQuestManager %d] 다음 레벨(씬) 이동"), __LINE__);
        }
    }
}
