// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\GameMode/FQGameModeUIInputInterface.h"
#include "FQUI\UI\FQSoulSelectScreenWidget.h"
#include "FQGameMode_SoulSelect.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_SoulSelect : public AGameModeBase, public IFQGameModeUIInputInterface
{
	GENERATED_BODY()
	
public:
	AFQGameMode_SoulSelect();

	// 인터페이스 가상 함수
	virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) override;
	virtual void CancelInteraction(int32 ControllerId) override;
	virtual void SelectInteraction(int32 ControllerId) override;
	virtual void MenuInteraction() override;
	virtual void StartGame() override;
	virtual void ExitGame() override;

protected:
	// 부모 가상 함수
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 일반 함수
	void CreatePlayer();

private:
	// Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQTitle, Meta = (AllowPrivateAccess = "true"))
	FName mTargetLevelName;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFQSoulSelectScreenWidget> mSoulSelectWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFQSoulSelectScreenWidget> mSoulSelectWidgetHandle;
	
	// Local Multi Player State
	TMap<uint8, EPlayerStateType> mPlayerStates;
	TMap<uint8, ESoulType> mPlayerSoulTypes;
};
