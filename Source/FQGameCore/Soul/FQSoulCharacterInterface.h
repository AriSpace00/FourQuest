// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQGameCore/Common.h"
#include "FQSoulCharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFQSoulCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class FQGAMECORE_API IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	virtual FTransform GetActorTransform() const abstract;
};
