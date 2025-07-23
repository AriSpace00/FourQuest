// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "Engine\TimerHandle.h"
#include "InputActionValue.h"

#include "FQGameCore\Player\FQPlayerControllerInterface.h"
#include "FQGameCore\Common.h"
#include "FQPlayerController_InGame.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQPlayerController_InGame : public APlayerController, public IFQPlayerControllerInterface
{
	GENERATED_BODY()	
	
public:
	AFQPlayerController_InGame();

	// 일반 함수
	void CreateUI();

	// 인터페이스 가상 함수
	virtual void ChangeToArmour(EArmourType InArmourType) override;
	virtual void ChangeToSoul() override;
	virtual void SetupInputComponent() override;

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;

private:
	// 일반 함수
	void CreatePlayerCharacterByClass(TSubclassOf<class AFQPlayerBase> CharacterClass, const FTransform& SpawnTransform);
	void CreateSoulCharacterByClass(TSubclassOf<class AFQSoulBase> CharacterClass, const FTransform& SpawnTransform);
	void SpawnPlayerUIActor();
	void UpdateHUDSetting();

	// UI 위젯 입력 함수
	void HandlePickButton();
	void HandleCancelButton();
	void HandleMenuButton();
	void HandleMoveTriggered(const FInputActionValue& Value);
	void HandleMoveCompleted(const FInputActionValue& Value);
	void DoMove();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class USoundBase> mSelectButtonSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class USoundBase> mCancelButtonSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData)
	TObjectPtr<class USoundBase> mMoveButtonSound;

private:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFQPlayerInputDataAsset> mPlayerInputDataAsset;

	// UI
	UPROPERTY()
	TObjectPtr<class UFQPlayerHUDWidget> mPlayerHUDWidget;
	UPROPERTY()
	TObjectPtr<class AFQPlayerUIActor> mPlayerOverheadUIActor;

	// 갑옷 타입과 영혼 타입에 따라 생성할 소울 캐릭터와 갑옷 캐릭터
	UPROPERTY(EditAnywhere, Category = FQCharacter, Meta = (AllowPrivateAccess = "true"))
	TMap<EArmourType, TSubclassOf<class AFQPlayerBase>> mPlayerArmourCharacterClasses;
	UPROPERTY(EditAnywhere, Category = FQCharacter, Meta = (AllowPrivateAccess = "true"))
	TMap<ESoulType, TSubclassOf<class AFQSoulBase>> mPlayerSoulCharacterClasses;

private:
	uint8 mbIsMoveKeyHeld : 1;
	FVector2D mMoveDirection;
	FTimerHandle mRepeatMoveTimerHandle;
};