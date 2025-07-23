// Fill out your copyright notice in the Description page of Project Settings.


#include "FQKnightDataAsset.h"

UFQKnightDataAsset::UFQKnightDataAsset()
{
	mDefaultSpeed = 200.0f;
	mMaxHp = 200.0f;

	mBashDamage = 0.0f;
	mBashSpeed = 500.0f;
	mBashDuration = 0.5f;
	mBashCoolTime = 1.0f;
	mBashStrength = 500.0f;
	mBashEffectScaleFactor = 0.5f;

	mSwordAttackCoolTime = 0.0f;
	mSwordAttackWaitTime1 = 0.0f;
	mSwordAttackWaitTime2 = 0.0f;
	mSwordAttackDamage1 = 5.0f;
	mSwordAttackDamage2 = 10.0f;
	mSwordAttackDamage3 = 15.0f;
	mSwordAttackStrength = 200.0f;
	mSwordEffectScaleFactor1 = 0.3f;
	mSwordEffectScaleFactor2 = 0.3f;
	mSwordEffectScaleFactor3 = 0.3f;

	mShieldDamage = 0.0f;
	mShieldWalkRatio = 50.0f;
	mShieldStrength = 500.0f;
	mShieldCoolTime = 0.0f;
	mShieldRotationSpeed = 5.0f;
}
