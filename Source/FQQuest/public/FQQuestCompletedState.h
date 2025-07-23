// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQQuestStateBase.h"
#include "FQQuestCompletedState.generated.h"


UCLASS()
class FQQUEST_API UFQQuestCompletedState : public UFQQuestStateBase
{
	GENERATED_BODY()

public:
	UFQQuestCompletedState();

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
