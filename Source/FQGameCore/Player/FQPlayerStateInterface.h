// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore\Common.h"
#include "FQPlayerStateInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQPlayerStateInterface
{
	GENERATED_BODY()

public:
	virtual void SetSoulType(const ESoulType InSoulType) abstract;
	virtual ESoulType GetSoulType() const abstract;
	virtual void SetArmourType(const EArmourType InArmourType) abstract;
	virtual EArmourType GetArmourType() const abstract;
	virtual void SetSoulGauge(const int32 NewValue) abstract;
	virtual void AddSoulGauge(const int32 AddValue) abstract;
	virtual int32 GetSoulGauge() const abstract;
	virtual void SetMaxHp(const int32 NewValue) abstract;
	virtual int32 GetMaxHp() const abstract;
	virtual void SetHp(const int32 NewValue) abstract;
	virtual void AddHp(const int32 AddValue) abstract;
	virtual int32 GetHp() const abstract;
};
