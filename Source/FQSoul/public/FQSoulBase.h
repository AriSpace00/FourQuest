// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "InputActionValue.h"
#include "FQGameCore\Common.h"
#include "FQSoulBase.generated.h"

UCLASS()
class FQSOUL_API AFQSoulBase : public ACharacter, public IFQSoulCharacterInterface
{
	GENERATED_BODY()

public:
	AFQSoulBase();

	// Interface Class Function
	virtual FTransform GetActorTransform() const override;

protected:
	// Parent Class Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Input Funtion
	void Move(const FInputActionValue& Value);
	void StartDash();
	void CheckArmour();
	void AddSoulGauge();		// Test Function

	// Change Armour Function
	class IFQArmourInterface* CheckNearArmour();

	UFUNCTION()
	void OnEquipEffectFinished(class UNiagaraComponent* PSystem);


	// Collision
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class UFQPlayerInputDataAsset> mPlayerInputDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class UFQSoulDataAsset> mSoulDataAsset;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> mEquipEffectComponent;

	UPROPERTY(EditAnywhere, Category = FQEffects)
	TObjectPtr<class UNiagaraSystem> mEquipEffectSystem;

private:
	// 대시 변수
	uint8 mbIsDashing : 1;
	FVector mDashDirection;
	float	mDashTimer;
	float	mDashCoolTimer;

	// 갑옷 변수
	float mArmourChangeTimer;

	UPROPERTY()
	TMap<FString, TObjectPtr<UObject>> mArmours;

	UPROPERTY()
	TObjectPtr<UObject> mCurrentArmour;
};
