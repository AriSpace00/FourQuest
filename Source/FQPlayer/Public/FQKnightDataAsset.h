// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FQKnightDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQKnightDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFQKnightDataAsset();

	// Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "기본 이동 속도"))
	float mDefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ToolTip = "체력"))
	float mMaxHp;

	// Bash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 데미지"))
	float mBashDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 속도"))
	float mBashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A,  meta = (ToolTip = "[A] 시간"))
	float mBashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A,  meta = (ToolTip = "[A] 쿨타임"))
	float mBashCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A,  meta = (ToolTip = "[A] 밀어내는 힘"))
	float mBashStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 이펙트 크기"))
	float mBashEffectScaleFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A,  meta = (ToolTip = "[A] 속도감 그래프"))
	TObjectPtr<class UCurveFloat> mBashCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_A, meta = (ToolTip = "[A] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mBashAttackableTypes;
	
	// Sword Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 쿨타임"))
	float mSwordAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 1타 -> 2타 입력 대기 시간"))
	float mSwordAttackWaitTime1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 2타 -> 3타 입력 대기 시간"))
	float mSwordAttackWaitTime2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 1타 데미지"))
	float mSwordAttackDamage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 2타 데미지"))
	float mSwordAttackDamage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 3타 데미지"))
	float mSwordAttackDamage3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 3타 밀어내는 힘"))
	float mSwordAttackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X,  meta = (ToolTip = "[X] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mSwordAttackableTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 1타 이펙트 크기"))
	float mSwordEffectScaleFactor1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 2타 이펙트 크기"))
	float mSwordEffectScaleFactor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_X, meta = (ToolTip = "[X] 3타 이펙트 크기"))
	float mSwordEffectScaleFactor3;

	// Shield
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 데미지"))
	float mShieldDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R,  meta = (ToolTip = "[R] 이동 속도 감소량"))
	float mShieldWalkRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R,  meta = (ToolTip = "[R] 밀어내는 힘"))
	float mShieldStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R,  meta = (ToolTip = "[R] 쿨타임"))
	float mShieldCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R,  meta = (ToolTip = "[R] 회전 속도"))
	float mShieldRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill_R, meta = (ToolTip = "[R] 피격 대상 타입"))
	TArray<TEnumAsByte<ECollisionChannel>> mShieldAttackableTypes;
	
};
