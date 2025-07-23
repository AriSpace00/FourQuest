// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState.h"
#include "FQMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		AIC->ChangeState(TargetState);
		
	}
	return EBTNodeResult::Succeeded;
}
