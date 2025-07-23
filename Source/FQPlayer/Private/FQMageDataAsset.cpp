// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageDataAsset.h"

UFQMageDataAsset::UFQMageDataAsset()
{
	mDefaultSpeed = 200.0f;
	mMaxHp = 200.0f;

	mProjectileAttackCoolTime = 1.0f;
	mProjectileAttackWaitTime1 = 2.0f;
	mProjectileAttackWaitTime2 = 2.0f;
	mProjectileCount = 2;
	mProjectileSpeed = 1000.0f;
	mProjectileDuration = 1.0f;
	mProjectileStrength = 300.0f;
	mProjectileDamageAmount = 5.0f;

	mExplosionCoolTime = 2.0f;
	mExplosionCircleSpeed = 200.0f;
	mMinDistance = 200.0f;
	mMaxDistance = 500.0f;
	mMaxScale = 1.0f;
	mMinScale = 0.5f;
	mExplosionDamageAmount = 5.0f;

	mLaserRotationSpeed = 5.0f;
	mLaserDamageTime = 0.1f;
	mLaserDamageAmount = 3.0f;
	mLaserMinDuration = 3.0f;
	mLaserCoolTime = 2.0f;
}
