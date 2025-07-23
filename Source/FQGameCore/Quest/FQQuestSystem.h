#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FQGameCore\Common.h"
#include "FQQuestTable.h"
#include "FQQuestSystem.generated.h"

UCLASS()
class FQGAMECORE_API UFQQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFQQuestSystem();

	// 퀘스트 델리게이트
	FQ_QuestDelegate::FQQuestTriggerDelegate mQuestTriggerDelegate;
	FQ_QuestDelegate::FQQuestActiveDelegate mQuestActiveDelegate;
	FQ_QuestDelegate::FQQuestMonsterDelegate mMonsterQuestDelegate;
	FQ_QuestDelegate::FQQuestInteractionDelegate mInteractionDelegate;
	FQ_QuestDelegate::FQQuestClearDelegate mQuestClearDelegate;

	FFQQuestTable* GetQuestData(int32 QuestNumber);
	void ResetQuestData();

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TMap<int32, FFQQuestTable> mQuestTableData;
};
