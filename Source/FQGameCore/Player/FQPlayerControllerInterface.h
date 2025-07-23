// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore\Common.h"
#include "FQPlayerControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQPlayerControllerInterface
{
	GENERATED_BODY()

public:
	virtual void ChangeToArmour(EArmourType InArmourType) abstract;
	virtual void ChangeToSoul() abstract;
};
