// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerAnimInstance.h"
#include "FQMageAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQMageAnimInstance : public UFQPlayerAnimInstance
{
	GENERATED_BODY()

public:
	UFQMageAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	uint8 mbIsEnabledCircle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mage")
	uint8 mbIsEnabledLaser : 1;
};
