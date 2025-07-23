// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "FQPlayer/Public/FQPlayerActionState.h"

#include "FQPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API UFQPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFQPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class AFQPlayerBase> mOwner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> mMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector mVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float mGroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float mNormalizedSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 mbIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float mMovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 mbHitReacting : 1;
};
