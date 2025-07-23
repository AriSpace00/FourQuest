// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore\Common.h"
#include "FQGameInstanceInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

class FQGAMECORE_API IFQGameInstanceInterface
{
	GENERATED_BODY()

public:
	virtual FQ_InGameSetting::FVideoInfomation GetVideoInfomation() const abstract;
	virtual void SetVideoInfomation(const FQ_InGameSetting::FVideoInfomation InVideoInfomation) abstract;
	virtual FQ_InGameSetting::FAudioInfomation GetAudioInfomation() const abstract;
	virtual void SetAudioInfomation(const FQ_InGameSetting::FAudioInfomation InAudioInfomation) abstract;
	virtual int32 GetPlayerCount() const abstract;

	virtual bool GetOnWidget() const abstract;
	virtual void SetOnWidget(const bool bOnWidget) abstract;
};
