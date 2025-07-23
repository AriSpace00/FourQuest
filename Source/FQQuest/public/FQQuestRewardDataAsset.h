// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQQuestRewardDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestRewardDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UFQQuestRewardDataAsset();

	UPROPERTY(EditAnywhere, Category = FQQuest, meta = (ToolTip = "클리어 퀘스트 아이디"))
	uint32	mClearQuestID;

	UPROPERTY(EditAnywhere, Category = FQReward, meta = (ToolTip = "보상 : 생성 액터"))
	TArray<TSubclassOf<AActor>>	mRewordSpawnActor;
	UPROPERTY(EditAnywhere, Category = FQReward, meta = (ToolTip = "보상 : 다음 퀘스트 아이디 (퀘스트 웨이 포인터 필수! 웨이 포인터 위치에 퀘스트 오브젝트 생성)"))
	uint32 mNextQuestID;
	UPROPERTY(EditAnywhere, Category = FQReward, meta = (ToolTip = "보상 : 다음 레벨 이동 여부"))
	uint8 mbOpenNextLevel : 1;
	UPROPERTY(EditAnywhere, Category = FQReward, meta = (ToolTip = "보상 : 이동할 다음 레벨 이름"))
	FName mNextLevelName;
};
