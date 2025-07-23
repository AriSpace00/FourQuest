// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    if (AIC && Target)
    {
        AIC->SetFocus(Target);
        ACharacter* MyChar = Cast<ACharacter>(AIC->GetPawn());
        if (MyChar)
        {
            MyChar->GetCharacterMovement()->bUseControllerDesiredRotation = true;
            MyChar->GetCharacterMovement()->bOrientRotationToMovement = false;
        }
    }
	return EBTNodeResult::Succeeded;
}
