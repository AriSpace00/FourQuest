// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQUI/Quest/FQQuestPointWidget.h"
#include "FQQuestPoint.generated.h"

UCLASS()
class FQQUEST_API AFQQuestPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestPoint();

	// 일반 함수
	void SetVisible(bool IsVisible);

	// 겟셋 함수
	FORCEINLINE class UFQQuestPointWidget* GetQuestPointWidget() const { return mQuestPointWidget; }
	FORCEINLINE int32 GetQuestID() const { return mQuestID; }
	FORCEINLINE void SetQuestActive(bool QuestActive) { mbIsQuestActive = QuestActive; }

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 콜백 함수
	void UpdateQuestActive(int32 QuestID, bool bIsQuestActive);

protected:
	// 퀘스트 아이디
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQQuest)
	int32 mQuestID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQQuest)
	EQuestSignType mQuestSignType;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQUI)
	TSubclassOf<class UFQQuestPointWidget> mQuestPointWidgetClass;
	UPROPERTY()
	TObjectPtr<class UFQQuestPointWidget> mQuestPointWidget;

private:
	uint8 mbIsQuestActive : 1;
};
