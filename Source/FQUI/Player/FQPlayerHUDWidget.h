// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQPlayerHUDWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQPlayerHUDWidget();

    void UpdateArmourSkill(EArmourType InArmourType);
    void UpdateSoulGauge(float GaugeValue);
    void UpdateHpValue(float HpValue);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    void PlaySoulBurningAnimation(float DeltaTime);
    void UpdateHpDecraceValue(float DeltaTime);
    void LoadingSoulBurningTexture(FString Path, FString FileName, uint32 TextureSize, TArray<TSoftObjectPtr<UTexture2D>>& SoulTextureContainer);
    void UpdateSoulBurningAnimation(TArray<TSoftObjectPtr<UTexture2D>>& SoulAnimationKey);

protected:
    // Soul Burning Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul)
    TArray<TSoftObjectPtr<UTexture2D>> mBlueSoulBurningAnimations;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul)
    TArray<TSoftObjectPtr<UTexture2D>> mYellowSoulBurningAnimations;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul)
    TArray<TSoftObjectPtr<UTexture2D>> mRedSoulBurningAnimations;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul)
    TArray<TSoftObjectPtr<UTexture2D>> mGreenSoulBurningAnimations;

    // Armour Skill 이미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    TMap<EArmourType, TObjectPtr<UTexture2D>> mArmourTypeMap;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    TMap<EArmourType, TObjectPtr<UTexture2D>> mXSkillMap;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    TMap<EArmourType, TObjectPtr<UTexture2D>> mASkillMap;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    TMap<EArmourType, TObjectPtr<UTexture2D>> mRSkillMap;

private:
    float mElapsedTime;
    uint8 mbIsSoulBurning : 1;
    uint8 mCurrentFrameIndex;
    ESoulType mSoulType;

    float mHpPercent;
    float mHpDecrasePercent;

    // SoulBurning
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSoulBurning;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class URadialSlider> mSoulGauge;

    // Armour Skill
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mArmourType;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UHorizontalBox> mSkillBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_X;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_A;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> mSkill_R;

    // Hp
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UProgressBar> mHp;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UProgressBar> mHpDecrase;
};
