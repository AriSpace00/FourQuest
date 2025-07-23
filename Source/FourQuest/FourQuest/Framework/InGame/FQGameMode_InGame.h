// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQGameCore\Common.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
#include "FQGameCore\GameMode\FQGameModeUIInputInterface.h"
#include "FQGameMode_InGame.generated.h"

/**
 * 
 */
UCLASS()
class FOURQUEST_API AFQGameMode_InGame : public AGameModeBase, public IFQGameModeInterface, public IFQGameModeUIInputInterface
{
	GENERATED_BODY()
	
public:
	AFQGameMode_InGame();

    // 인터페이스 함수
    virtual FTransform GetMainCameraTransform() const override;

    // UI 인터페이스 함수
    virtual void StartGame() override;
    virtual void ExitGame() override;
    virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) override;
    virtual void CancelInteraction(int32 ControllerId) override;
    virtual void SelectInteraction(int32 ControllerId) override;
    virtual void MenuInteraction() override;
    
    // 겟셋 함수
    FORCEINLINE class AFQPlayerHUDManager* GetPlayerHUDManager() const { return mPlayerHUDManager; }
    FORCEINLINE class AFQMainCenterCamera* GetMainCamera() const { return mMainCamera; }

protected:
    // 부모 가상 함수
    virtual void BeginPlay() override;

    // 일반 함수
    void SettingCamera(); // 카메라 및 HUD Widget 세팅
    void CreatePlayer();  // 로컬 멀티 플레이어 셍성

private:
    // 맵에 생성할 영혼 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul, Meta = (AllowPrivateAccess = "true"))
    TMap<ESoulType, TSubclassOf<class AFQSoulBase>> mSoulPlayersType;

    // 플레이어 HUD WIdget Manager
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQHUDWidget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AFQPlayerHUDManager> mPlayerHUDManager;

    // 씬 카메라
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FQCamera, Meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class AFQMainCenterCamera> mMainCamera;

    // 게임 메뉴
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQUI, Meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UFQInGamePauseUI> mGamePauseUIClass;
    UPROPERTY()
    TObjectPtr<class UFQInGamePauseUI> mGamePauseUI;
};
