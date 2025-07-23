// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQMonsterBase.h"
#include "FQMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class FQMONSTER_API AFQMeleeMonster : public AFQMonsterBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay()override;

	void Hit();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	virtual void TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength) override;
};
