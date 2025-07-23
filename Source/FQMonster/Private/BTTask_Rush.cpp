


#include "BTTask_Rush.h"
#include "FQBossMonster.h"
#include "FQMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Rush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQBossMonster* Monster = Cast<AFQBossMonster>(AIC->GetPawn());
		if (Monster)
		{
			AActor* Target = Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
			if (Target)
			{
				Monster->Rush();
				return EBTNodeResult::InProgress;
			}
		}
	}
    return EBTNodeResult::Succeeded;
}

void UBTTask_Rush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQBossMonster* Monster = Cast<AFQBossMonster>(AIC->GetPawn());
		if (Monster)
		{
			if (Monster->mMonsterState != EMonsterState::Rush)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}
