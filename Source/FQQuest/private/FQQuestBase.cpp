#include "FQQuestBase.h"

#include "FQQuestStartedState.h"
#include "FQQuestCompletedState.h"
#include "FQQuestInProgressState.h"
#include "FQUI\Quest\FQQuestWidget.h"
#include "FQGameCore\Quest\FQQuestSystem.h"
#include "FQMonsterKillQuest.h"
#include "FQInteractionQuest.h"
#include "FQQuestPoint.h"
#include "FQQuestTriggerVolume.h"

#include "Kismet\GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

AFQQuestBase::AFQQuestBase()
	: mQuestID()
	, mCurrentState()
	, mCurrentStateType(EQuestStateType::Started)
	, mQuestCurrentConditionNumber()
	, mQuestClearConditionNumber()
{
	ConstructorHelpers::FClassFinder<UUserWidget> QuestWidgetRef(TEXT("/Game/Blueprints/Quest/WBP_QuestWidget.WBP_QuestWidget_C"));
	if (QuestWidgetRef.Class)
	{
		mQuestWidget = CreateWidget<UFQQuestWidget>(GetWorld(), QuestWidgetRef.Class);
	}
}

void AFQQuestBase::BeginPlay()
{
	Super::BeginPlay();

	// UI
	mQuestWidget->SetQuestDescriptionText(mDescription);

	// State
	mCurrentState = NewObject<UFQQuestStartedState>(this);
	mCurrentState->SetOwnerQuestObject(this);
	mCurrentState->EnterState();
	UpdateQuestCondition(0);

	// 퀘스트 시스템 탐색
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		// UI 델리게이트 등록
		QuestSystem->mQuestActiveDelegate.AddUObject(this, &AFQQuestBase::UpdateQuestActive);

		// 서브 퀘스트 생성
		FFQQuestTable* MyQuestData = QuestSystem->GetQuestData(mQuestID);
		for (int32 SubQuestID : MyQuestData->SubQuestList)
		{
			CreateSubQuest(SubQuestID);
		}

		// UI 활성화
		mQuestWidget->UpdateQuestActive(MyQuestData->mbIsActive);
	}

	// 퀘스트 포인트 찾아서 표시하기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFQQuestPoint::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		AFQQuestPoint* QuestPoint = Cast<AFQQuestPoint>(Actor);
		if (QuestPoint && QuestPoint->GetQuestID() == mQuestID)
		{
			QuestPoint->SetVisible(true);
			break;
		}
	}
}


void AFQQuestBase::UpdateQuest(float DeltaTime)
{
	// 퀘스트 상태 업데이트
	if (mCurrentState)
	{
		mCurrentState->UpdateState(DeltaTime);
	}

	// 서브 퀘스트 업데이트
	if (mCurrentStateType != EQuestStateType::Exit)
	{
		for (auto& SubQuest : mSubQuestList)
		{
			if (IsValid(SubQuest.Value))
			{
				SubQuest.Value->UpdateQuest(DeltaTime);
			}
		}
	}

	// UI 업데이트
	switch (mCurrentStateType)
	{
	case EQuestStateType::Started:
	{
		// 퀘스트 포인트 찾아서 표시하기
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFQQuestPoint::StaticClass(), FoundActors);
		for (auto Actor : FoundActors)
		{
			AFQQuestPoint* QuestPoint = Cast<AFQQuestPoint>(Actor);
			if (QuestPoint && QuestPoint->GetQuestID() == mQuestID)
			{
				QuestPoint->SetVisible(true);
				break;
			}
		}

		// 퀘스트 UI 업데이트
		UFQQuestStartedState* MyState = Cast<UFQQuestStartedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
		}
	}
	break;
	case EQuestStateType::Exit:
	{
		// 퀘스트 UI 업데이트
		UFQQuestCompletedState* MyState = Cast<UFQQuestCompletedState>(mCurrentState);
		if (MyState)
		{
			mQuestWidget->UpdateQuestStateAnimation(MyState->GetDurationValue());
			mQuestWidget->UpdateQuestStateCheckBoxAnimation(MyState->GetDurationValue());
		}
	}
	break;
	}
}

void AFQQuestBase::SetNewState(const EQuestStateType NewState)
{
	mCurrentStateType = NewState;
	switch (mCurrentStateType)
	{
	case EQuestStateType::Started:
	{

	}
	break;
	case EQuestStateType::InPrograss:
	{
		// 상태 업데이트
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestInProgressState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();
	}
	break;
	case EQuestStateType::Exit:
	{
		// 상태 업데이트
		mCurrentState->ExitState();
		mCurrentState = NewObject<UFQQuestCompletedState>(this);
		mCurrentState->SetOwnerQuestObject(this);
		mCurrentState->EnterState();

		// 서브 퀘스트 삭제
		RemoveSubQuest();

		// 퀘스트 클리어 데이터 저장
		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem)
		{
			QuestSystem->GetQuestData(mQuestID)->mbIsQuestClear = true;
		}

		// 퀘스트 포인트 Off
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFQQuestPoint::StaticClass(), FoundActors);
		for (auto Actor : FoundActors)
		{
			AFQQuestPoint* QuestPoint = Cast<AFQQuestPoint>(Actor);
			if (QuestPoint && QuestPoint->GetQuestID() == mQuestID)
			{
				QuestPoint->SetVisible(false);
				break;
			}
		}

		// 퀘스트 트리거 볼륨 삭제
		TArray<AActor*> FoundTriggerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFQQuestTriggerVolume::StaticClass(), FoundTriggerActors);
		for (auto Actor : FoundTriggerActors)
		{
			Actor->Destroy();
		}
	}
	break;
	case EQuestStateType::End:
	{
		mCurrentState = nullptr;

		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem)
		{
			QuestSystem->mQuestClearDelegate.Broadcast(mQuestID);
		}
	}
	break;
	}

	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestState(mCurrentStateType);
	}
}

void AFQQuestBase::UpdateQuestCondition(int32 AddConditionNumber)
{
	if (mQuestCurrentConditionNumber == mQuestClearConditionNumber)
	{
		return;
	}

	mQuestCurrentConditionNumber += AddConditionNumber;
	if (mQuestCurrentConditionNumber >= mQuestClearConditionNumber)
	{
		SetNewState(EQuestStateType::Exit);
	}
	else if (mQuestCurrentConditionNumber < 0)
	{
		mQuestCurrentConditionNumber = 0;
	}

	// UI 퀘스트 현 상태 업데이트
	if (mQuestWidget)
	{
		mQuestWidget->UpdateQuestCondition(mQuestClearConditionNumber > mQuestCurrentConditionNumber ? mQuestCurrentConditionNumber : mQuestClearConditionNumber, mQuestClearConditionNumber);
	}
}

void AFQQuestBase::RemoveSubQuest()
{
	for (auto& SubQuest : mSubQuestList)
	{
		if (IsValid(SubQuest.Value))
		{
			SubQuest.Value->SetActorHiddenInGame(true);
			SubQuest.Value->SetActorEnableCollision(false);
			SubQuest.Value->SetLifeSpan(0.1f); // 약간의 지연 후 파괴
		}
	}
	mSubQuestList.Empty();

	if (mQuestWidget)
	{
		mQuestWidget->RemoveSubQuestWidget();
	}
}

void AFQQuestBase::CreateSubQuest(int32 QuestID)
{
	// 퀘스트 시스템 탐색
	UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
	if (QuestSystem)
	{
		FFQQuestTable* QuestData = QuestSystem->GetQuestData(QuestID);

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
			MonsterKillQuest->SetOwnerQuest(this);
			MonsterKillQuest->FinishSpawning(FTransform());
			mSubQuestList.Emplace(QuestData->QuestNumber, *MonsterKillQuest);

			UFQQuestWidget* MyQuestWidget = MonsterKillQuest->GetQuestWidget();
			if (MyQuestWidget)
			{
				mQuestWidget->AddSubQuestListWidget(MyQuestWidget);
			}
		}
		break;
		case EQuestType::Interaction:
		{
			AFQInteractionQuest* InteractionQuest = GetWorld()->SpawnActorDeferred<AFQInteractionQuest>(AFQInteractionQuest::StaticClass(), FTransform());
			InteractionQuest->SetQuestID(QuestData->QuestNumber);
			InteractionQuest->SetQuestClearConditionNumber(QuestData->QuestClearConditionsNumber);
			InteractionQuest->SetQuestDescription(QuestData->QuestDescription);
			InteractionQuest->SetQuestInteractionType(QuestData->QuestInteractionType);
			InteractionQuest->SetOwnerQuest(this);
			InteractionQuest->FinishSpawning(FTransform());
			mSubQuestList.Emplace(QuestData->QuestNumber, *InteractionQuest);

			UFQQuestWidget* MyQuestWidget = InteractionQuest->GetQuestWidget();
			if (MyQuestWidget)
			{
				mQuestWidget->AddSubQuestListWidget(MyQuestWidget);
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

void AFQQuestBase::UpdateQuestActive(int32 QuestID, bool bIsQuestActive)
{
	if (mQuestID == QuestID)
	{
		mQuestWidget->UpdateQuestActive(bIsQuestActive);
	}
}
