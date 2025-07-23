// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQMonsterBase.h"
#include "FQBossMonsterDataAsset.h"
#include "FQBossMonster.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API AFQBossMonster : public AFQMonsterBase
{
	GENERATED_BODY()

public:
	AFQBossMonster();

	virtual void BeginPlay() override;

	virtual void Attack() override;

	void StrikeAttack();

	void Growl();

	void PrepareRush();

	void RushApplyDamage(ACharacter* Character);

	void Rush();

	void EndRush();

	void SelectAttack();

	void SelectSkill();

	void Choose();

	void Hit(FVector AttackPos);

	void ApplyDamageToTarget();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	void OnChargeHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	UFQBossMonsterDataAsset* mBossMonsterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	USoundCue* mRushHitSoundCue;

private:
	FTimerHandle mSPPIncreaseTimer;
	FTimerHandle mDownDecreaseTimer;

	FVector mRushDirection;

	float mDownGauge;

	float mOriginalBrakingFrictionFactor;
	float mOriginalGravityScale;

	float mSPP;
};
