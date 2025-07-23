// Fill out your copyright notice in the Description page of Project Settings.

#include "FQ_GameInstance_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FQPlayerController_InGame.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "GameFramework\GameUserSettings.h"

UFQ_GameInstance_InGame::UFQ_GameInstance_InGame()
	: mbOnWidget(true)
{
	// 기본 0번째 플레이어 정보 생성
	FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation;
	PlayerInfomation.bSpawnLocalPlayer = true;
	PlayerInfomation.mSoulType = ESoulType::Sword;
	PlayerInfomation.mArmourType = EArmourType::End;
	mLocalMultiPlayerArr.Add(0, PlayerInfomation);
}

void UFQ_GameInstance_InGame::SetVideoInfomation(const FQ_InGameSetting::FVideoInfomation InVideoInfomation)
{
	mVideoInfomation = InVideoInfomation;

	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		if (mVideoInfomation.mbIsFullScreen)
		{
			Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else
		{
			Settings->SetFullscreenMode(EWindowMode::Windowed);
		}

		FQ_InGameSetting::EResolutionType ResolutionType = mVideoInfomation.mCurrentResolutionType;
		switch (ResolutionType)
		{
		case FQ_InGameSetting::EResolutionType::Resolution_1920x1080:
			Settings->SetScreenResolution(FIntPoint(1920, 1080));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1600x900:
			Settings->SetScreenResolution(FIntPoint(1600, 900));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1280x720:
			Settings->SetScreenResolution(FIntPoint(1280, 720));
			break;
		case FQ_InGameSetting::EResolutionType::Resolution_1080x600:
			Settings->SetScreenResolution(FIntPoint(1080, 680));
			break;
		}

		Settings->ApplySettings(false); // true면 GPU 리스타트 포함
	}
}

void UFQ_GameInstance_InGame::SetAudioInfomation(const FQ_InGameSetting::FAudioInfomation InAudioInfomation)
{
	mAudioInfomation = InAudioInfomation;

	if (!mSoundMix || !mBGMSC || !mSFXSC)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] SoundMix or SoundClass가 유효하지 않습니다!!"), __LINE__);
		return;
	}

	// 음소거 적용
	float FullVolume = mAudioInfomation.mbIsFullVolumMute ? 0.0f : mAudioInfomation.mFullVolumValue;
	float BGMVolume = mAudioInfomation.mbIsBGMVolumMute ? 0.0f : mAudioInfomation.mBGMVolumValue;
	float SFXVolume = mAudioInfomation.mbIsSFXVolumMute ? 0.0f : mAudioInfomation.mSFXVolumValue;

	// Sound Class에 실제 볼륨 적용
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		mSoundMix,
		mBGMSC,
		FullVolume * BGMVolume,
		1.0f,
		0.0f,
		true
	);

	UGameplayStatics::SetSoundMixClassOverride(
		this,
		mSoundMix,
		mSFXSC,
		FullVolume * SFXVolume,
		1.0f,
		0.0f,
		true
	);

	// 실제로 반영
	UGameplayStatics::PushSoundMixModifier(this, mSoundMix);
}

int32 UFQ_GameInstance_InGame::GetPlayerCount() const
{
	int32 PlayerCount = 0;
	for (auto Player : mLocalMultiPlayerArr.Array())
	{
		if (Player.Value.bSpawnLocalPlayer)
		{
			PlayerCount++;
		}
	}
	return PlayerCount;
}

void UFQ_GameInstance_InGame::Init()
{
	Super::Init();

	// 사운드 믹스 추가
	if (mSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, mSoundMix);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] mSoundMix가 유효하지 않습니다!!"), __LINE__);
	}

	if (!mBGMSC || !mSFXSC)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] mBGMSC, mSFXSC가 유효하지 않습니다!!"), __LINE__);
	}
}


void UFQ_GameInstance_InGame::SetBGMSoundVolum(float VolumValue)
{
	// 브금 볼륨 조절
	if (mSoundMix, mBGMSC)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, mSoundMix, mBGMSC, VolumValue, 1.0f, 0.5f);
		UGameplayStatics::PushSoundMixModifier(this, mSoundMix);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] mSoundMix, mBGMSC가 유효하지 않습니다!!"), __LINE__);
	}
}

void UFQ_GameInstance_InGame::SetSFXSoundVolum(float VolumValue)
{
	// 효과음 볼륨 조절
	if (mSoundMix, mSFXSC)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, mSoundMix, mSFXSC, VolumValue, 1.0f, 0.5f);
		UGameplayStatics::PushSoundMixModifier(this, mSoundMix);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] mSoundMix, mSFXSC가 유효하지 않습니다!!"), __LINE__);
	}
}


void UFQ_GameInstance_InGame::SavePlayerInfomation(APawn* Player)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] Player is nullptr!!"), __LINE__);
		return;
	}

	AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(Player->GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] AFQPlayerController_InGame is nullptr!!"), __LINE__);
		return;
	}

	AFQPlayerState_InGame* PlayerState = Cast<AFQPlayerState_InGame>(Player->GetPlayerState());
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] AFQPlayerState_InGame is nullptr!!"), __LINE__);
		return;
	}

	UWorld* World = Player->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQ_GameInstance_InGame %d] World is nullptr!!"), __LINE__);
		return;
	}

	int32 PlayerControllerId = 0;

	// 현재 생성된 로컬 멀티 플레이어 체크
	for (ULocalPlayer* LP : LocalPlayers)
	{
		if (LP && LP->PlayerController == PC)
		{
			PlayerControllerId = LP->GetControllerId();
			break;
		}
	}

	// 생성된 로컬 멀티 플레이어 정보 저장
	if (mLocalMultiPlayerArr.Find(PlayerControllerId) == nullptr)
	{
		FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation;
		PlayerInfomation.mSoulType = PlayerState->GetSoulType();
		PlayerInfomation.mArmourType = PlayerState->GetArmourType();
		mLocalMultiPlayerArr.Add(PlayerControllerId, PlayerInfomation);
	}
	else
	{
		FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation = mLocalMultiPlayerArr[PlayerControllerId];
		PlayerInfomation.mSoulType = PlayerState->GetSoulType();
		PlayerInfomation.mArmourType = PlayerState->GetArmourType();
		mLocalMultiPlayerArr[PlayerControllerId] = PlayerInfomation;
	}
}

void UFQ_GameInstance_InGame::SavePlayerInfomation(int32 PlayerControllerId, FQ_InGameSetting::FQLocalMultiPlayerInfomation InPlayerInfomation)
{
	// 생성된 로컬 멀티 플레이어 정보 저장
	if (mLocalMultiPlayerArr.Find(PlayerControllerId) == nullptr)
	{
		FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation;
		PlayerInfomation.bSpawnLocalPlayer = InPlayerInfomation.bSpawnLocalPlayer;
		PlayerInfomation.mSoulType = InPlayerInfomation.mSoulType;
		PlayerInfomation.mArmourType = InPlayerInfomation.mArmourType;
		mLocalMultiPlayerArr.Add(PlayerControllerId, PlayerInfomation);
	}
	else
	{
		FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation = mLocalMultiPlayerArr[PlayerControllerId];
		PlayerInfomation.bSpawnLocalPlayer = InPlayerInfomation.bSpawnLocalPlayer;
		PlayerInfomation.mSoulType = InPlayerInfomation.mSoulType;
		PlayerInfomation.mArmourType = InPlayerInfomation.mArmourType;
		mLocalMultiPlayerArr[PlayerControllerId] = PlayerInfomation;
	}
}