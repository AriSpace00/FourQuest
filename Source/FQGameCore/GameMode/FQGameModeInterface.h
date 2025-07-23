// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameModeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQGameModeInterface
{
	GENERATED_BODY()

public:
	virtual FTransform GetMainCameraTransform() const abstract;
};
