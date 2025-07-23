// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Actor.h"
#include "FQPlayerHUDManager.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQPlayerHUDManager : public AActor
{
	GENERATED_BODY()
	
public:
	AFQPlayerHUDManager();

	// 일반 함수
	void AddPlayerController(APlayerController* NewPlayer, UUserWidget* PlayerHUDWidget);

private:
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUserWidget> mHUDWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UUserWidget>> mPlayerHUDs;
};
