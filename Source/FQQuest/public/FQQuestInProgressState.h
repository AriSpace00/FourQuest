// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestInProgressState.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestInProgressState : public UFQQuestStateBase
{
	GENERATED_BODY()

public:
	UFQQuestInProgressState();

	// 부모 가상 함수
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

private:

};
