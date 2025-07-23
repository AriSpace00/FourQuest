// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQMageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQMageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQMageDataAsset();

	// Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "기본 이동 속도"))
	float mDefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "체력"))
	float mMaxHp;

	// Projectile Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 쿨타임"))
	float mProjectileAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 1타 -> 2타 입력 대기 시간"))
	float mProjectileAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 2타 -> 1타 입력 대기 시간"))
	float mProjectileAttackWaitTime2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 관통 수"))
	int32 mProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 발사 속도"))
	float mProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 지속 시간"))
	float mProjectileDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 밀어내는 힘"))
	float mProjectileStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 투사체 데미지"))
	float mProjectileDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mProjectileAttackableTypes;

	// Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 쿨타임"))
	float mExplosionCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 마법진 이동 속도"))
	float mExplosionCircleSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 마법진 크기 감소 시작 거리"))
	float mMinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 마법진 크기가 최솟값이 되는 거리"))
	float mMaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 마법진 최대 크기"))
	float mMaxScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 마법진 최소 크기"))
	float mMinScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 데미지"))
	float mExplosionDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mExplosionAttackableTypes;

	// Laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 회전 속도"))
	float mLaserRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 데미지 틱 주기"))
	float mLaserDamageTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 데미지"))
	float mLaserDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 최소 지속 시간"))
	float mLaserMinDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 쿨타임"))
	float mLaserCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mLaserAttackableTypes;

};
