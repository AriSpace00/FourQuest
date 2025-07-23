#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FQGameCore\Common.h"
#include "FQQuestTable.generated.h"

USTRUCT(BlueprintType)
struct FQGAMECORE_API FFQQuestTable : public FTableRowBase
{
	GENERATED_BODY()
	
	FFQQuestTable() 
		: QuestNumber()
		, QuestType(EQuestType::None)
		, QuestDescription()
		, QuestMonsterType(EQuestMonsterType::None)
		, QuestInteractionType(EQuestInteractionType::None)
		, QuestClearConditionsNumber()
		, mbIsQuestClear(false)
		, mbIsActive(false)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestMonsterType QuestMonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestInteractionType QuestInteractionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestMonsterGroupName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestClearConditionsNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SubQuestList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> NextQuestList; 

	// 퀘스트 클리어 여부
	uint8 mbIsQuestClear : 1;
	uint8 mbIsActive : 1;
};