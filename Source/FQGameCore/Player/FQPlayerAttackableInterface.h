// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FQPlayerAttackableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFQPlayerAttackableInterface : public UInterface
{
	GENERATED_BODY()
};

// 플레이어가 공격할 수 있는 액터 인터페이스
class FQGAMECORE_API IFQPlayerAttackableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength) PURE_VIRTUAL(IFQPlayerAttackableInterface::TakePushByPlayer, );

};
