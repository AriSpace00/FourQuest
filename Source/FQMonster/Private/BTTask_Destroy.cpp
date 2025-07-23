// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Destroy.h"
#include "FQMonsterBase.h"
#include "FQMonsterAIController.h"

EBTNodeResult::Type UBTTask_Destroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQMonsterBase* Monster = Cast<AFQMonsterBase>(AIC->GetPawn());
		if (Monster)
		{
			Monster->Destroy();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}