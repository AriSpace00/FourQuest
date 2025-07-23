// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQMageCircle.generated.h"

UCLASS()
class FQPLAYER_API AFQMageCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFQMageCircle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> mVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> mEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDecalComponent> mDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAudioComponent> mCircleAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAudioComponent> mExplosionAudio;

	void ActivateEffect();
	void SetScale(float ScaleFactor);

	UFUNCTION()
	void OnEffectFinished(class UNiagaraComponent* PSystem);

	void PlayCircleAudio();
	void StopCircleAudio();

	void PlayExplosionAudio();

private:
	FVector mInitBoxExtent;
	FVector mInitDecalSize;
};
