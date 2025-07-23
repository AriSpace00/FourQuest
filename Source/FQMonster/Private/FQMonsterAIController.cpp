// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMonsterAIController.h"
#include "FQMeleeMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AFQMonsterAIController::AFQMonsterAIController()
{
	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    mSightConfig->SightRadius = 1000.0f;
    mSightConfig->LoseSightRadius = 1200.0f;
    mSightConfig->PeripheralVisionAngleDegrees = 90.0f;
    mSightConfig->SetMaxAge(5.0f);
    mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
    mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    mSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    
    mAIPerception->ConfigureSense(*mSightConfig);
    mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());

    mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AFQMonsterAIController::OnTargetPerceptionUpdated);

    mBehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("Behavior Tree"));
    
    SetGenericTeamId(FGenericTeamId(1));
}

void AFQMonsterAIController::SetTargetActor(AActor* Actor)
{
    AFQMonsterBase* Monster = GetPawn<AFQMonsterBase>();
    if (Monster)
    {
        GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
        Monster->mTargetActor = Actor;
        if (AFQMeleeMonster* Melee = Cast<AFQMeleeMonster>(Monster))
        {
            Melee->GetCharacterMovement()->MaxWalkSpeed = Melee->mMonsterDataAsset->mChaseSpeed;
        }
        ChangeState(EMonsterState::Chase);
    }
}

void AFQMonsterAIController::ChangeState(EMonsterState State)
{
    //ai컨트롤러에서 블랙보드와 몬스터의 상태를 다 바꿔줌
    AFQMonsterBase* Monster = GetPawn<AFQMonsterBase>();
    if (Monster&&Monster->mMonsterState!=EMonsterState::Death)
    {
        Monster->mMonsterState = State;
        GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)State);
    }
}

void AFQMonsterAIController::ChangeTargetActor(AActor* Actor)
{
    AFQMonsterBase* Monster = Cast<AFQMonsterBase>(GetPawn());
    if (Monster)
    {
        Monster->mTargetActor = Actor;
        GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
    }
}

void AFQMonsterAIController::PickRandomTarget()
{
    TArray<AActor*> PerceivedActors;
    mAIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

    // 조건: 플레이어 태그가 있는 액터만
    TArray<AActor*> PlayerCandidates;
    for (AActor* Actor : PerceivedActors)
    {
        if (Actor->Tags.Contains(FName("Player")))
        {
            PlayerCandidates.Add(Actor);
        }
    }

    if (PlayerCandidates.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, PlayerCandidates.Num() - 1);
        ChangeTargetActor(PlayerCandidates[Index]);
    }
}

void AFQMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

}

void AFQMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(mBehaviorTree)
    {
        UBlackboardComponent* RawBlackboard = Blackboard.Get();
        UseBlackboard(mBehaviorTree->BlackboardAsset, RawBlackboard);
        Blackboard = RawBlackboard;
        GetBlackboardComponent()->SetValueAsVector(TEXT("OriginLocation"), InPawn->GetActorLocation());

        RunBehaviorTree(mBehaviorTree);
    }
}

void AFQMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    AFQMonsterBase* Monster = Cast<AFQMonsterBase>(GetPawn());
    if (Monster->mMonsterState == EMonsterState::Death)
    {
        return;
    }
    
    if (Stimulus.WasSuccessfullySensed())
    {
        if (Actor->Tags.Contains(FName("Player")))
        {
            if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
            {
                if (!Actor->IsA(AFQMonsterBase::StaticClass()))
                {
                    Monster->ManagerSetTargetActor(Actor);
                }
            }
        }
    }
    else
    {
        if(!IsValid(Monster->mTargetActor))
        {
            Monster->ManagerSetTargetActor(nullptr);
            AFQMonsterAIController* AIC = GetInstigatorController<AFQMonsterAIController>();
            if (AIC)
            {
                AIC->SetTargetActor(nullptr);
                AIC->ChangeState(EMonsterState::Idle);
                if (AFQMeleeMonster* Melee = Cast<AFQMeleeMonster>(Monster))
                {
                    Melee->GetCharacterMovement()->MaxWalkSpeed = Melee->mMonsterDataAsset->mMoveSpeed;
                }
            }
        }
    }
}
