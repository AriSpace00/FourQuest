// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "FQMonsterBase.h"
#include "FQMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQMonsterBase* Monster = Cast<AFQMonsterBase>(AIC->GetPawn());
		if (Monster)
		{
			AActor* Target = Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
			if(Target)
			{
				Monster->Attack();
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
