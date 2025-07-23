// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerState_InGame.h"
#include "FQGameMode_InGame.h"
#include "FQUI\Player\FQPlayerHUDWidget.h"

AFQPlayerState_InGame::AFQPlayerState_InGame() 
	: mSoulType(ESoulType::End)
	, mArmourType(EArmourType::End)
	, mMaxSoulGaue(100)
	, mCurrentSoulGauge(0)
	, mMaxHp(100)
	, mCurrentHp(mMaxHp)
{
}

void AFQPlayerState_InGame::AddSoulGauge(const int32 AddValue)
{
	mCurrentSoulGauge += AddValue; 
	if (mCurrentSoulGauge > mMaxSoulGaue)
	{
		mCurrentSoulGauge = mMaxSoulGaue;
	}

	float Value = (float)mCurrentSoulGauge / (float)mMaxSoulGaue;
	mSoulGaugeDelegate.Broadcast(Value);
}

void AFQPlayerState_InGame::SetSoulGauge(const int32 NewValue)
{
	mCurrentSoulGauge = NewValue;
	if (mCurrentSoulGauge > mMaxSoulGaue)
	{
		mCurrentSoulGauge = mMaxSoulGaue;
	}

	float Value = (float)mCurrentSoulGauge / (float)mMaxSoulGaue;
	mSoulGaugeDelegate.Broadcast(Value);
}

void AFQPlayerState_InGame::SetMaxHp(const int32 NewValue)
{
	mMaxHp = NewValue; 
	mCurrentHp = mMaxHp;
	mPlayerHpDelegate.Broadcast(1.f);
}

void AFQPlayerState_InGame::SetHp(const int32 NewValue)
{
	mCurrentHp = NewValue;
	if (mCurrentHp <= 0)
	{
		mCurrentHp = 0;
		mPlayerHpDelegate.Broadcast(0);
		mPlayerDeadDelegate.Broadcast(true);
		return;
	}
	else if (mCurrentHp > mMaxHp)
	{
		mCurrentHp = mMaxHp;
	}

	float Value = (float)mCurrentHp / (float)mMaxHp;
	mPlayerHpDelegate.Broadcast(Value);
	mPlayerDeadDelegate.Broadcast(false);
	UE_LOG(LogTemp, Log, TEXT("[AFQPlayerState_InGame %d] SetHp 함수 실행 (MaxHp : %d, CurrentHp : %d, HpValue : %f)"), __LINE__, mMaxHp, mCurrentHp, Value);
}

void AFQPlayerState_InGame::AddHp(const int32 AddValue)
{
	mCurrentHp += AddValue;
	if (mCurrentHp <= 0)
	{
		mCurrentHp = 0;
		mPlayerHpDelegate.Broadcast(0);
		mPlayerDeadDelegate.Broadcast(true);
		return;
	}
	else if (mCurrentHp > mMaxHp)
	{
		mCurrentHp = mMaxHp;
	}

	float Value = (float)mCurrentHp / (float)mMaxHp;
	mPlayerHpDelegate.Broadcast(Value);
	mPlayerDeadDelegate.Broadcast(false);
	UE_LOG(LogTemp, Log, TEXT("[AFQPlayerState_InGame %d] AddHp 함수 실행 (MaxHp : %d, CurrentHp : %d, HpValue : %f)"), __LINE__, mMaxHp, mCurrentHp, Value);
}
