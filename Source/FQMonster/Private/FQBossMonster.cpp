// Fill out your copyright notice in the Description page of Project Settings.


#include "FQBossMonster.h"
#include "FQMonsterManager.h"
#include "FQMonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

AFQBossMonster::AFQBossMonster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFQBossMonster::BeginPlay()
{
	Super::BeginPlay();
	mbCanPush = false;
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFQBossMonster::OnChargeHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFQBossMonster::OnAttackOverlap);

	mOriginalBrakingFrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
	mOriginalGravityScale = GetCharacterMovement()->GravityScale;
	if (mGroupID == FName("None"))
	{
		mMonsterType = EQuestMonsterType::BossMonster;
	}
	else
	{
		mMonsterType = EQuestMonsterType::MonsterGroup;
	}
	if (mBossMonsterDataAsset)
	{
		mCurrentHP = mBossMonsterDataAsset->mMaxHP;
		mCurrentHPPercent = mCurrentHP / mBossMonsterDataAsset->mMaxHP;
		mSPP = mBossMonsterDataAsset->mInitSPP;
		GetCharacterMovement()->MaxWalkSpeed = mBossMonsterDataAsset->mChaseSpeed;
		mAttackRange = mBossMonsterDataAsset->mAttackRange;
	}

	mDownGauge = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(mDownDecreaseTimer, [this]() { if (mDownGauge <= 0)return; mDownGauge -= 0.5f; }, 1.0f, true);
}

void AFQBossMonster::Attack()
{
	if (mAttackMontage)
	{
		const TArray<FName> Sections = { TEXT("Attack1"), TEXT("Attack2") };
		int32 Index = FMath::RandRange(0, Sections.Num() - 1);
		PlayAnimMontage(mAttackMontage, 1.0f, Sections[Index]);
	}
}

void AFQBossMonster::StrikeAttack()
{
	if (mAttackMontage)
	{
		FName SectionName = TEXT("Attack3");
		PlayAnimMontage(mAttackMontage, 1.0f, SectionName);
	}
}

void AFQBossMonster::Growl()
{
	ChangeState(EMonsterState::Growl);
	if (Manager)
	{
		Manager->ChangeTargetActor(mGroupID, mTargetActor);
	}
}

void AFQBossMonster::PrepareRush()
{
	ChangeState(EMonsterState::Rush);
	if (mTargetActor)
	{
		mRushDirection = (mTargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		FRotator Rot = mRushDirection.Rotation();
		FRotator NewRot = FRotator(0.f, Rot.Yaw, 0.f);
		SetActorRotation(NewRot);
	}
}

void AFQBossMonster::RushApplyDamage(ACharacter* Character)
{
	FVector ToTarget = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector KnockbackDir;
	float CrossZ = FVector::CrossProduct(GetVelocity().GetSafeNormal(), ToTarget).Z;
	if (CrossZ > 0)
	{
		// 오른쪽 넉백
		KnockbackDir = FVector::CrossProduct(FVector::UpVector, GetVelocity().GetSafeNormal()).GetSafeNormal();
	}
	else
	{
		// 왼쪽 넉백
		KnockbackDir = FVector::CrossProduct(GetVelocity().GetSafeNormal(), FVector::UpVector).GetSafeNormal();
	}
	float KnockbackForce = mBossMonsterDataAsset->mRushStrength;
	UGameplayStatics::ApplyDamage(Character, mBossMonsterDataAsset->mRushDamage, GetController(), this, UDamageType::StaticClass());
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->LaunchCharacter(KnockbackDir * KnockbackForce, true, true);
	if(mRushHitSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mRushHitSoundCue, Character->GetActorLocation());
	}
}

void AFQBossMonster::Rush()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->MaxFlySpeed = 1500.f;
	TArray<AActor*> Overlapped;
	GetCapsuleComponent()->GetOverlappingActors(Overlapped, AActor::StaticClass());

	for (AActor* Actor : Overlapped)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("[Overlapped] TakeRush"));
			RushApplyDamage(Character);
		}
	}

	GetCharacterMovement()->Velocity = mRushDirection * mBossMonsterDataAsset->mRushSpeed;

	

	//bIsCharging = true;
}

void AFQBossMonster::EndRush()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->BrakingFrictionFactor = mOriginalBrakingFrictionFactor;
	GetCharacterMovement()->GravityScale = mOriginalGravityScale;
	ChangeState(EMonsterState::Chase);
}

void AFQBossMonster::SelectAttack()
{
	int32 RandInt = FMath::RandRange(0, 1);
	if (RandInt)
	{
		StrikeAttack();
	}
	else
	{
		Attack();
	}
}

void AFQBossMonster::SelectSkill()
{
	int32 RandInt = FMath::RandRange(1, 10);
	if (RandInt <= 4)
	{
		PrepareRush();
	}
	else if (RandInt >= 8)
	{
		Growl();
	}
	else
	{
		SelectAttack();
	}
}

void AFQBossMonster::Choose()
{
	int32 RandInt = FMath::RandRange(1, 100);
	if (RandInt < mSPP)
	{
		SelectSkill();
		mSPP = mBossMonsterDataAsset->mInitSPP;
	}
	else
	{
		SelectAttack();
		mSPP += mBossMonsterDataAsset->mAttackSPP;
	}
}

void AFQBossMonster::Hit(FVector AttackPos)
{
	if (mHitMontage)
	{
		FVector ForwardVector = GetActorForwardVector();
		float DotResult = FVector::DotProduct(ForwardVector, AttackPos);
		if (DotResult > 0.5)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitFront"));
		}
		else if (DotResult < -0.5)
		{
			PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitBack"));
		}
		else
		{
			if (FVector::CrossProduct(ForwardVector, AttackPos).Z > 0)
			{
				PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitLeft"));
			}
			else
			{
				PlayAnimMontage(mHitMontage, 1.0f, TEXT("HitRight"));
			}
		}
	}
}

void AFQBossMonster::ApplyDamageToTarget()
{
	TArray<AActor*> Overlapped;
	mAttackBox->GetOverlappingActors(Overlapped, AActor::StaticClass());

	for (AActor* Target : Overlapped)
	{
		if (Target && Target != this)
		{
			UGameplayStatics::ApplyDamage(Target, mBossMonsterDataAsset->mAttackPower, GetController(), this, UDamageType::StaticClass());
			if (mAttackHitSoundCue)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), mAttackHitSoundCue, mAttackBox->GetComponentLocation());
			}
		}
	}
}

float AFQBossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[BossMonster] TakeDamage"));

		//AIC->ChangeState(EMonsterState::Chase);
		if(mMonsterState!=EMonsterState::Down)
		{
			mCurrentHP = mCurrentHP - DamageAmount;
			//AIC->ChangeState(EMonsterState::Hit);
			
		}
		else
		{
			mCurrentHP = mCurrentHP - (DamageAmount*mBossMonsterDataAsset->mDownHitDamage);
			Hit((DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal());
		}
		mCurrentHPPercent = mCurrentHP / mBossMonsterDataAsset->mMaxHP;
		mDownGauge += 5.0f;
		if (mDownGauge >= 100)
		{
			AIC->ChangeState(EMonsterState::Down);
			mDownGauge = 0;
		}
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

void AFQBossMonster::OnChargeHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!OtherActor->ActorHasTag("Floor"))
	{
		EndRush();
	}
}

void AFQBossMonster::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(mMonsterState==EMonsterState::Rush)
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			RushApplyDamage(Character);
		}
	}
}
