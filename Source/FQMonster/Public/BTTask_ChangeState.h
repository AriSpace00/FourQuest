// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FQMonsterBase.h"
#include "BTTask_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API UBTTask_ChangeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	float TargetDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Condition")
	EMonsterState TargetState;
};
