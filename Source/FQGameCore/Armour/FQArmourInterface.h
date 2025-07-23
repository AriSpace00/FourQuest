// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore/Common.h"
#include "FQArmourInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQArmourInterface : public UInterface
{
	GENERATED_BODY()
};

class FQGAMECORE_API IFQArmourInterface
{
	GENERATED_BODY()

public:
	virtual void PickArmour() abstract;
	virtual EArmourType GetArmourType() const abstract;
	virtual void SetNearestArmour(bool IsTrue) abstract;
	virtual FTransform GetActorTransform() const abstract;

	virtual int32 GetArmourHp() const abstract;
};
