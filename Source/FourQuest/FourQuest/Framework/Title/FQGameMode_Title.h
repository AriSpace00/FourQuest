// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\GameMode\FQGameModeUIInputInterface.h"
#include "FQGameMode_Title.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_Title : public AGameModeBase, public IFQGameModeUIInputInterface
{
	GENERATED_BODY()
	
public:
	AFQGameMode_Title();

	// 인터페이스 가상 함수
	virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) override;
	virtual void CancelInteraction(int32 ControllerId) override;
	virtual void SelectInteraction(int32 ControllerId) override;
	virtual void MenuInteraction() override;
	virtual void StartGame() override;
	virtual void ExitGame() override;

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	// Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQTitle, Meta = (AllowPrivateAccess = "true"))
	FName mTargetLevelName;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UFQTitleScreenWidget> mTitleWidgetClass;
	UPROPERTY()
	TObjectPtr<class UFQTitleScreenWidget> mTitleWidgetHandle;
};
