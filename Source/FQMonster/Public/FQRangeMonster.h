// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQMonsterBase.h"
#include "FQRangeMonsterDataAsset.h"
#include "FQRangeMonster.generated.h"

class AFQRangeMonsterProjectile;

/**
 * 
 */
UCLASS()
class FQMONSTER_API AFQRangeMonster : public AFQMonsterBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void ProjectileAttack();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AFQRangeMonsterProjectile> mProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	UFQRangeMonsterDataAsset* mRangeMonsterDataAsset;
};
