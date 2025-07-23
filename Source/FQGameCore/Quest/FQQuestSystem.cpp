#include "FQQuestSystem.h"

UFQQuestSystem::UFQQuestSystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> LoadedTable(TEXT("DataTable'/Game/Data/DT_QuestTable.DT_QuestTable'"));
	if (LoadedTable.Succeeded())
	{
		UDataTable* QuestTable = LoadedTable.Object;
		
		// 모든 Row 로드
		const TArray<FName> RowNames = QuestTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			static const FString ContextString(TEXT("QuestTable Load Context"));
			if (FFQQuestTable* RowData = QuestTable->FindRow<FFQQuestTable>(RowName, ContextString))
			{
				mQuestTableData.Add(RowData->QuestNumber, *RowData); // 복사
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQQuestSystem %d] Failed to load QuestTable"), __LINE__);
	}
}

void UFQQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

FFQQuestTable* UFQQuestSystem::GetQuestData(int32 QuestNumber)
{
	// 퀘스트 업데이트 체크 및 반환
	auto MyQuestData = mQuestTableData.Find(QuestNumber);
	if (MyQuestData != nullptr)
	{
		MyQuestData->mbIsActive = true;
		for (int32 SubQusetID : MyQuestData->SubQuestList)
		{
			FFQQuestTable* SubQuestData = mQuestTableData.Find(SubQusetID);
			if (SubQuestData && !SubQuestData->mbIsQuestClear)
			{
				MyQuestData->mbIsActive = false;
				break;
			}
		}
		
		// 퀘스트 활성화 델리게이트 호출
		mQuestActiveDelegate.Broadcast(QuestNumber, MyQuestData->mbIsActive);
		return MyQuestData;
	}
	else
	{
		return nullptr;
	}
}

void UFQQuestSystem::ResetQuestData()
{
	// 퀘스트 리셋
	for (auto& QuestDataPair : mQuestTableData)
	{
		QuestDataPair.Value.mbIsQuestClear = false;
		QuestDataPair.Value.mbIsActive = false;
	}
}
