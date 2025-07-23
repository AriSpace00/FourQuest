// Fill out your copyright notice in the Description page of Project Settings.


#include "FQRangeMonster.h"
#include "FQRangeMonsterProjectile.h"
#include "FQMonsterAIController.h"

#include "Components/CapsuleComponent.h"

void AFQRangeMonster::BeginPlay()
{
    Super::BeginPlay();
    mbCanPush = false;
    if (mGroupID == FName("None"))
    {
        mMonsterType = EQuestMonsterType::CommonRangedMonster;
    }
    else
    {
        mMonsterType = EQuestMonsterType::MonsterGroup;
    }
    if (mRangeMonsterDataAsset)
    {
        mCurrentHP = mRangeMonsterDataAsset->mMaxHP;
        mCurrentHPPercent = mCurrentHP / mRangeMonsterDataAsset->mMaxHP;
        mAttackRange = mRangeMonsterDataAsset->mAttackRange;
    }
}
void AFQRangeMonster::ProjectileAttack()
{
    if (!mTargetActor || !mProjectile) return;

    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50.f); // 약간 위로
    FRotator SpawnRotation = (FVector(mTargetActor->GetActorLocation() - SpawnLocation)).Rotation();
    SpawnRotation.Pitch = 0.0f;
    SpawnRotation.Roll = 0.0f;


    FActorSpawnParameters Params;
    Params.Instigator = this;
    Params.Owner = this;

    AFQRangeMonsterProjectile* Projectile = GetWorld()->SpawnActor<AFQRangeMonsterProjectile>(mProjectile, SpawnLocation, SpawnRotation, Params);
    if (Projectile)
    {
        if (mRangeMonsterDataAsset)
        {
            Projectile->SetDamage(mRangeMonsterDataAsset->mAttackPower); // 예시 데미지
            Projectile->SetProjectileSpeed(mRangeMonsterDataAsset->mProjectileSpeed);
        }
    }
}

float AFQRangeMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
    {
        UE_LOG(LogTemp, Warning, TEXT("[RnageMonster] TakeDamage"));

        if (DamageCauser->Tags.Contains(FName("Player")))
        {
            AIC->ChangeTargetActor(DamageCauser);
        }
        mCurrentHP = mCurrentHP - DamageAmount;
        mCurrentHPPercent = mCurrentHP / mRangeMonsterDataAsset->mMaxHP;
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
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            AIC->ChangeState(EMonsterState::Death);
            DeleteActor(mGroupID, this);
        }
    }

    return DamageAmount;
}
