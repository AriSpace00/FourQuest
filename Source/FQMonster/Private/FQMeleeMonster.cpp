// Fill out your copyright notice in the Description page of Project Settings.


#include "FQMeleeMonster.h"
#include "FQMonsterAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void AFQMeleeMonster::BeginPlay()
{
	Super::BeginPlay();
	if (mGroupID == FName("None"))
	{
		mMonsterType = EQuestMonsterType::CommonMeleeMonster;
	}
	else
	{
		mMonsterType = EQuestMonsterType::MonsterGroup;
	}
	if (mMonsterDataAsset)
	{
		mAttackRange = mMonsterDataAsset->mAttackRange;
	}
}

void AFQMeleeMonster::Hit()
{
	if (mHitMontage&&mCurrentHP>0)
	{
		if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
		{
			AIC->StopMovement();
		}
		if (GetVelocity().Size2D() > 0)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("Push"));
			return;
		}
		const TArray<FName> Sections = { TEXT("Hit1"), TEXT("Hit2") };
		int32 Index = FMath::RandRange(0, Sections.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Hit Animation"))
		PlayAnimMontage(mHitMontage, 1.0f, Sections[Index]);
	}
}

float AFQMeleeMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[MeleeMonster] TakeDamage"));

		
		mCurrentHP = mCurrentHP - DamageAmount;
		mCurrentHPPercent = mCurrentHP / mMonsterDataAsset->mMaxHP;
		if (mCurrentHP <= 0)
		{
			UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
			if (QuestSystem)
			{
				if (mMonsterType == EQuestMonsterType::MonsterGroup)
				{
					QuestSystem->mMonsterQuestDelegate.Broadcast(EQuestMonsterType::MonsterGroup, mGroupID);
				}
				else
				{
					QuestSystem->mMonsterQuestDelegate.Broadcast(mMonsterType, "");
				}
			}
			AIC->StopMovement();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AIC->ChangeState(EMonsterState::Death);
			DeleteActor(mGroupID, this);
		}
		else
		{
			if(DamageCauser->Tags.Contains(FName("Player")))
			{
				AIC->SetTargetActor(DamageCauser);
				ManagerSetTargetActor(DamageCauser);
			}
			Hit();
		}
	}

	return DamageAmount;
}

void AFQMeleeMonster::TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength)
{
	Super::TakePushByPlayer(Target, Direction, Strength);
	if (mMonsterState != EMonsterState::Hit)
	{
		Hit();
	}
}