// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Common.h"
#include "FQQuestBase.generated.h"

UCLASS()
class FQQUEST_API AFQQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestBase();

	// 가상 함수
	virtual void Execute() PURE_VIRTUAL(AFQQuestBase::Execute, );
	virtual void UpdateQuest(float DeltaTime);

	// 일반 함수
	void SetNewState(const EQuestStateType NewState);
	void UpdateQuestCondition(int32 AddConditionNumber);
	void RemoveSubQuest();

	// 겟셋 함수
	FORCEINLINE class UFQQuestWidget* GetQuestWidget() const { return mQuestWidget; }
	FORCEINLINE EQuestStateType GetCurrentState() const { return mCurrentStateType; }
	FORCEINLINE void SetOwnerQuest(AFQQuestBase* OwnerQuest) { mOwnerQuest = OwnerQuest; }
	FORCEINLINE void SetQuestID(const int32 QuestID) { mQuestID = QuestID; }
	FORCEINLINE int32 GetQuestID() const { return mQuestID; }
	FORCEINLINE void SetQuestClearConditionNumber(const int32 QuestClearConditionNumber) { mQuestClearConditionNumber = QuestClearConditionNumber; }
	FORCEINLINE int32 GetQuestClearConditionNumber() const { return mQuestClearConditionNumber; }
	FORCEINLINE void SetQuestDescription(const FString Description) { mDescription = Description; }
	FORCEINLINE FString GetQuestDescription() const { return mDescription; }
	FORCEINLINE void AddConditionQuestNumber(const int32 ConditionQuest) { mConditionQuests.Emplace(ConditionQuest); }
	FORCEINLINE const TArray<int32>& GetConditionQuest() { return mConditionQuests; }

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	// 일반 함수
	void CreateSubQuest(int32 QuestID);

	// 콜백 함수
	void UpdateQuestActive(int32 QuestID, bool bIsQuestActive);

protected:
	// 퀘스트 내용
	int32 mQuestID;
	FString mDescription;

	// 퀘스트 클리어 조건
	int32 mQuestClearConditionNumber;
	int32 mQuestCurrentConditionNumber;

	// 다음 퀘스트 목록
	TArray<int32> mConditionQuests;

	// 현재 상태
	EQuestStateType mCurrentStateType;

	// 현재 퀘스트 상태
	UPROPERTY()
	TObjectPtr<class UFQQuestStateBase> mCurrentState;

	// 서브 퀘스트 목록
	UPROPERTY()
	TMap<int32, TObjectPtr<AFQQuestBase>> mSubQuestList;
	UPROPERTY()
	TWeakObjectPtr<AFQQuestBase> mOwnerQuest;

	// 퀘스트 포인터
	UPROPERTY()
	TObjectPtr<class AFQQuestPoint> mQuestPointer;

	// UI
	UPROPERTY()
	TObjectPtr<class UFQQuestWidget> mQuestWidget;
};
