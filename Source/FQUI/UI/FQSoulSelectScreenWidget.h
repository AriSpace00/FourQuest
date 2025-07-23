// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQSoulSelectScreenWidget.generated.h"

UENUM()
enum class EPlayerStateType : uint8
{
	NoSelect = 0,
	SoulSelect,
	SoulComplate
};

UCLASS()
class FQUI_API UFQSoulSelectScreenWidget : public UFQUserWidget
{
	GENERATED_BODY()

public:
	UFQSoulSelectScreenWidget();

	// Input Function
	void WidgetInput(ESoulType SoulType, int32 ControllerId);
	void UpdatePlayerState(EPlayerStateType PlayerStateType, ESoulType SoulType, int32 ControllerId);

	// GetSet Function
	FORCEINLINE bool GetStartNextLevel() const { return mStartNextLevel; }
	FORCEINLINE void SetAllReady(bool AllReady) { mAllReady = AllReady; }

protected:
	// Parent Function
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Common Function
	void MoveIndex(ESoulType SoulType, int32 ControllerId);


private:
	uint8 mStartNextLevel : 1;
	uint8 mAllReady : 1;
	float mReadyElapsedTime;
	uint8 mCurrentReadyCount;
	float mAnimationElapsedTime;
	uint8 mCurrentFrameIndex;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> mNoSelectSoulArr;
	UPROPERTY()
	TArray<TObjectPtr<class UFQSoulSelectUI>> mSoulSelectUIArr;
	UPROPERTY()
	TArray<TObjectPtr<class UFQSoulComplateUI>> mSoulComplateUIArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQReady, Meta = (AllowPrivateAccess = "true"))
	TArray<FText> mReadyCountText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mReadyCount;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mReadyText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mReadyBackGround;
};
