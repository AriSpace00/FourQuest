// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FQGameCore\Player\FQPlayerStateInterface.h"
#include "FQPlayerState_InGame.generated.h"

UCLASS()
class FOURQUEST_API AFQPlayerState_InGame : public APlayerState, public IFQPlayerStateInterface
{
	GENERATED_BODY()
	
public:
    AFQPlayerState_InGame();

public:
    // 델리게이트
    FQ_PlayerStateDelegate::FQSoulTypeChangeDelegate mSoulChangeDelegate;
    FQ_PlayerStateDelegate::FQArmourTypeChangeDelegate mArmourChangeDelegate;
    FQ_PlayerStateDelegate::FQSoulGaugeSettingDelegate mSoulGaugeDelegate;
    FQ_PlayerStateDelegate::FQPlayerHpUpdateDelegate mPlayerHpDelegate;
    FQ_PlayerStateDelegate::FQPlayerDeadDelegate mPlayerDeadDelegate;

    // 인터페이스 가상 함수
    virtual ESoulType GetSoulType() const override { return mSoulType; }
    virtual void SetSoulType(const ESoulType NewType) override { mSoulType = NewType; mSoulChangeDelegate.Broadcast(mSoulType); }
    virtual EArmourType GetArmourType() const override { return mArmourType; }
    virtual void SetArmourType(const EArmourType NewType) override { mArmourType = NewType; mArmourChangeDelegate.Broadcast(mArmourType); }
    virtual void AddSoulGauge(const int32 AddValue) override;
    virtual void SetSoulGauge(const int32 NewValue) override;
    virtual int32 GetSoulGauge() const override { return mCurrentSoulGauge; }
    virtual void SetMaxHp(const int32 NewValue) override;
    virtual int32 GetMaxHp() const override { return mMaxHp; }
    virtual void SetHp(const int32 NewValue) override;
    virtual void AddHp(const int32 AddValue) override;
    virtual int32 GetHp() const override { return mCurrentHp; }

protected:
    // Soul
    UPROPERTY(BlueprintReadOnly, Category = FQSoul)
    ESoulType mSoulType;
    UPROPERTY(BlueprintReadWrite, Category = FQSoul)
    int32 mCurrentSoulGauge;
    int32 mMaxSoulGaue;

    // 체력
    UPROPERTY(BlueprintReadWrite, Category = FQState)
    int32 mMaxHp;
    int32 mCurrentHp;

    // Armour
    UPROPERTY(BlueprintReadOnly, Category = FQArmour)
    EArmourType mArmourType;
};
