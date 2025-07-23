// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChooseAttack.h"
#include "FQBossMonster.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_ChooseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		AFQBossMonster* Boss = Cast<AFQBossMonster>(AIC->GetPawn());
		if (Boss)
		{
			Boss->Choose();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
