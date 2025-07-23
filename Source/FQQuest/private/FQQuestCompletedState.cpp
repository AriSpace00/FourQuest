#include "FQQuestCompletedState.h"

#include "FQQuestBase.h"

UFQQuestCompletedState::UFQQuestCompletedState()
	: mComplatedTime(3.f)
	, mValueCutLine(2.f)
	, mDurationTime()
{
}

void UFQQuestCompletedState::EnterState()
{
}

void UFQQuestCompletedState::UpdateState(float DeltaTime)
{
	mDurationTime += DeltaTime;
	if (mDurationTime > mComplatedTime)
	{
		AFQQuestBase* MyQuestActor = GetOwnerQuestObject();
		MyQuestActor->SetNewState(EQuestStateType::End);
	}
}

void UFQQuestCompletedState::ExitState()
{
}
