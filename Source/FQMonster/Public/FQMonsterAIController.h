// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "FQMonsterBase.h"
#include "FQMonsterAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class FQMONSTER_API AFQMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFQMonsterAIController();

	void SetTargetActor(AActor* Actor);

	void ChangeState(EMonsterState State);

	void ChangeTargetActor(AActor* Actor);

	void PickRandomTarget();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "BehaviorTree")
	UBehaviorTree* mBehaviorTree;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AIPerception")
	UAIPerceptionComponent* mAIPerception;
	
	UPROPERTY()
	UAISenseConfig_Sight* mSightConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
