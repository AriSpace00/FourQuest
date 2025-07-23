// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (AIC)
    {
        AIC->ClearFocus(EAIFocusPriority::Gameplay);
        AIC->StopMovement();
        ACharacter* MyChar = Cast<ACharacter>(AIC->GetPawn());
        if (MyChar)
        {
            MyChar->GetCharacterMovement()->bUseControllerDesiredRotation = false;
            MyChar->GetCharacterMovement()->bOrientRotationToMovement = false;
        }
       
    }
    return EBTNodeResult::Succeeded;
}