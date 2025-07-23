// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQMageStaff.generated.h"

UCLASS()
class FQPLAYER_API AFQMageStaff : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFQMageStaff();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> mLaserEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> mHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> mLaserEffectLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAudioComponent> mLaserAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAudioComponent> mHitAudio;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateLaserEffect(const FVector& End);
	void DeactivateLaserEffect();
	const FVector GetLaserEffectLocation();

	void ActivateHitEffect(const FVector& End);
	void DeactivateHitEffect();

	void PlayLaserAudio();
	void StopLaserAudio();
};
