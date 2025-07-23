// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FQGameCore\GameInstance\FQGameInstanceInterface.h"
#include "FQGameCore\Common.h"
#include "FQ_GameInstance_InGame.generated.h"


UCLASS()
class FOURQUEST_API UFQ_GameInstance_InGame : public UGameInstance, public IFQGameInstanceInterface
{
	GENERATED_BODY()
	
	using PlayerControllerId = int32;

public:
	UFQ_GameInstance_InGame();

	// 인터페이스 가상 함수
	virtual void SetVideoInfomation(const FQ_InGameSetting::FVideoInfomation InVideoInfomation) override;
	virtual void SetAudioInfomation(const FQ_InGameSetting::FAudioInfomation InAudioInfomation) override;
	virtual FQ_InGameSetting::FVideoInfomation GetVideoInfomation() const override { return mVideoInfomation; }
	virtual FQ_InGameSetting::FAudioInfomation GetAudioInfomation() const override { return mAudioInfomation; }
	virtual int32 GetPlayerCount() const override;
	virtual bool GetOnWidget() const override { return mbOnWidget; }
	virtual void SetOnWidget(const bool bOnWidget) override { mbOnWidget = bOnWidget; }

	// 플레이어 정보 저장
	void SavePlayerInfomation(APawn* Player);
	void SavePlayerInfomation(int32 PlayerControllerId, FQ_InGameSetting::FQLocalMultiPlayerInfomation InPlayerInfomation);

	// 일반 함수
	FORCEINLINE TMap<PlayerControllerId, FQ_InGameSetting::FQLocalMultiPlayerInfomation> GetLocalMultiPlayerInfomation() const { return mLocalMultiPlayerArr; }

protected:
	// 부모 가상 함수
	virtual void Init() override;

	// 일반 함수
	void SetBGMSoundVolum(float VolumValue);
	void SetSFXSoundVolum(float VolumValue);

protected:
	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSound)
	TObjectPtr<class USoundMix> mSoundMix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSound)
	TObjectPtr<class USoundClass> mBGMSC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSound)
	TObjectPtr<class USoundClass> mSFXSC;

private:
	// UI
	int8 mbOnWidget : 1;

	// 게임 환경설정 정보
	TMap<PlayerControllerId, FQ_InGameSetting::FQLocalMultiPlayerInfomation> mLocalMultiPlayerArr;
	FQ_InGameSetting::FVideoInfomation mVideoInfomation;
	FQ_InGameSetting::FAudioInfomation mAudioInfomation;
};
