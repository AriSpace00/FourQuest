// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Armour/FQArmourInterface.h"
#include "FQArmourBase.generated.h"

UCLASS()
class FQARMOUR_API AFQArmourBase : public AActor, public IFQArmourInterface
{
	GENERATED_BODY()
	
public:	
	AFQArmourBase();

	// Interface Function
	virtual void PickArmour() override;
	virtual void SetNearestArmour(bool IsTrue) override;
	virtual EArmourType GetArmourType() const override;
	virtual FTransform GetActorTransform() const override;
	virtual int32 GetArmourHp() const override { return mArmourHp; }

protected:
	// Parent Class Funtion
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = FQBox)
	TObjectPtr<class UBoxComponent> mTrigger;
	UPROPERTY(EditAnywhere, Category = FQBox)
	TObjectPtr<class UStaticMeshComponent> mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
	EArmourType mArmourType;

	UPROPERTY()
	TObjectPtr<class UFQWidgetComponent> mArmourWidget;

	UPROPERTY(EditAnywhere, Category = FQArmourStat)
	int32 mArmourHp;
};
