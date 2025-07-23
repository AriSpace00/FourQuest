// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomLocation.h"
#include "FQMonsterAIController.h"
#include "FQMonsterBase.h"
#include "FQMonsterDataAsset.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}
	AFQMonsterBase* Monster = AIC->GetPawn<AFQMonsterBase>();	
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}
	UFQMonsterDataAsset* DA = Monster->mMonsterDataAsset;
	if (!DA)
	{
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation RandomLocation;
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("OriginLocation"));

	if (NavSys->GetRandomReachablePointInRadius(Origin, DA->mPatrolDistance, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
