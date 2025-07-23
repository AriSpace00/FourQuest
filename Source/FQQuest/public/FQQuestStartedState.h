// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestStartedState.generated.h"

/**
 * 
 */
UCLASS()
class FQQUEST_API UFQQuestStartedState : public UFQQuestStateBase
{
	GENERATED_BODY()
	
public:
	UFQQuestStartedState();

	// 부모 가상 함수
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;
	
	// 겟셋 함수
	FORCEINLINE float GetDurationValue() const { return mValueCutLine > mDurationTime ? 0.f : (mDurationTime - mValueCutLine) / (mComplatedTime - mValueCutLine); }

private:
	float mValueCutLine;
	float mComplatedTime;
	float mDurationTime;
};
