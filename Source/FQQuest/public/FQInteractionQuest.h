// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestBase.h"
#include "FQGameCore\Common.h"
#include "FQInteractionQuest.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API AFQInteractionQuest : public AFQQuestBase
{
	GENERATED_BODY()
	
public:
	AFQInteractionQuest();
	
	// 부모 가상 함수
	virtual void Execute() override;
	virtual void UpdateQuest(float DeltaTime) override;

	// 겟셋 함수
	FORCEINLINE void SetQuestInteractionType(const EQuestInteractionType InteractionType) { mInteractionType = InteractionType; }
	FORCEINLINE EQuestInteractionType GetQuestInteractionType() const { return mInteractionType; }

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	// 콜백 함수
	void TryUpdateQuestState(EQuestInteractionType InteractionType, int32 QuestConditionCount);

private:
	EQuestInteractionType mInteractionType;
};
