#include "FQQuestStartedState.h"

#include "FQQuestBase.h"

UFQQuestStartedState::UFQQuestStartedState()
	: mComplatedTime(3.f)
	, mValueCutLine(2.f)
	, mDurationTime()
{
}

void UFQQuestStartedState::EnterState()
{
}

void UFQQuestStartedState::UpdateState(float DeltaTime)
{
	mDurationTime += DeltaTime;
	if (mDurationTime > mComplatedTime)
	{
		AFQQuestBase* MyQuestActor = GetOwnerQuestObject();
		MyQuestActor->SetNewState(EQuestStateType::InPrograss);
	}
}

void UFQQuestStartedState::ExitState()
{
}
