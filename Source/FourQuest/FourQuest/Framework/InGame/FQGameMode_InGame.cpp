// Fill out your copyright notice in the Description page of Project Settings.

#include "FQGameMode_InGame.h"

#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "FQ_GameInstance_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FQPlayerController_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager/FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul/public/FQSoulBase.h"
#include "FQUI/UI/FQInGamePauseUI.h"

AFQGameMode_InGame::AFQGameMode_InGame()
{
    DefaultPawnClass = nullptr;
}

void AFQGameMode_InGame::BeginPlay()
{
    Super::BeginPlay();

    // 카메라 생성 및 세팅
    SettingCamera();
    // 플레이어 생성
    CreatePlayer();

    // 게임 인스턴스 UI 세팅
	IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
	if (MyGameInstance)
	{
        MyGameInstance->SetOnWidget(false);
	}

    // 게임 메뉴 UI 생성
    if (mGamePauseUIClass)
    {
        mGamePauseUI = CreateWidget<UFQInGamePauseUI>(GetWorld(), mGamePauseUIClass);
        mGamePauseUI->AddToViewport();
        mGamePauseUI->SetVisibility(ESlateVisibility::Hidden);
        mGamePauseUI->mPauseUIDelegate.BindUObject(this, &AFQGameMode_InGame::MenuInteraction);
    }
}

void AFQGameMode_InGame::SettingCamera()
{
    // 로컬 플레이어를 추가했을 시에 카메라 분할(x) 세팅  
    if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
    {
        ViewportClient->SetForceDisableSplitscreen(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d] Faild Create Player : Current Player Count >= Max Player Count!!"), __LINE__);
    }

    // 메인 카메라 생성
    mMainCamera = GetWorld()->SpawnActor<AFQMainCenterCamera>(AFQMainCenterCamera::StaticClass());
    if (mMainCamera == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] Faild Create MainCamera"), __LINE__);
    }

    // 화면 HUD Widget 세팅
    mPlayerHUDManager = GetWorld()->SpawnActor<AFQPlayerHUDManager>(AFQPlayerHUDManager::StaticClass());
}

void AFQGameMode_InGame::CreatePlayer()
{
    UFQ_GameInstance_InGame* GameInstance = Cast<UFQ_GameInstance_InGame>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_InGame %d] UFQ_GameInstance_InGame Is Not Vaild!!"), __LINE__);
        return;
    }

    // PlayerStart 찾기
    TArray<AActor*> FoundStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);

    APlayerStart* StartActor = Cast<APlayerStart>(FoundStarts[0]);
    if (!StartActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayerStart"));
        return;
    }

    // 위치 및 회전 가져오기
    FVector SpawnLocation = StartActor->GetActorLocation();
    FRotator SpawnRotation = StartActor->GetActorRotation();

    auto PlayerInfomations = GameInstance->GetLocalMultiPlayerInfomation();

    for (auto& Infomation : PlayerInfomations)
    {
        const int32 ControllerId = Infomation.Key;
        FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation = Infomation.Value;

        // 생성할 로컬 플레이어인가
        if (!PlayerInfomation.bSpawnLocalPlayer)
        {
            continue;
        }

        // 생성할 영혼 타입 지정
        if (mSoulPlayersType.Num() > (uint8)PlayerInfomation.mSoulType)
        {
            DefaultPawnClass = mSoulPlayersType[PlayerInfomation.mSoulType];
        }

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Added Player %d"), __LINE__, ControllerId);

            // 플레이어 스테이트 클래스에 소울 타입 지정
            AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(NewPlayer->GetPlayerController(GetWorld()));
            AFQPlayerState_InGame* FQPlayerState = PC->GetPlayerState<AFQPlayerState_InGame>();
            if (FQPlayerState)
            {
                FQPlayerState->SetSoulType(PlayerInfomation.mSoulType);
                FQPlayerState->SetArmourType(EArmourType::End);
                FQPlayerState->SetSoulGauge(0);
            }

            // UI 생성
            PC->CreateUI();
        }
        else
        {
            // 생성할 로컬 플레이어인가
            if (!PlayerInfomation.bSpawnLocalPlayer)
            {
                continue;
            }

            AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(GetGameInstance()->GetLocalPlayers()[ControllerId]->GetPlayerController(GetWorld()));
            if (!PC)
            {
                UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d]"), __LINE__);
                continue;
            }

            // 기존 Pawn 제거 (선택 사항)
            if (PC->GetPawn())
            {
                PC->GetPawn()->Destroy();
            }

            // 생성할 영혼 타입 지정
            if (mSoulPlayersType.Num() > (uint8)PlayerInfomation.mSoulType)
            {
                DefaultPawnClass = mSoulPlayersType[PlayerInfomation.mSoulType];
            }

            // 새로운 캐릭터 생성
            AFQSoulBase* NewPawn = GetWorld()->SpawnActor<AFQSoulBase>(mSoulPlayersType[PlayerInfomation.mSoulType], SpawnLocation + FVector(100 * ControllerId), SpawnRotation);
            if (!NewPawn)
            {
                UE_LOG(LogTemp, Error, TEXT("[FQGameMode_InGame %d]Failed to spawn Pawn for ControllerId %d"), __LINE__, ControllerId);
                return;
            }

            // Possess 처리
            PC->Possess(NewPawn);

            // PlayerState 설정
            if (AFQPlayerState_InGame* PS = PC->GetPlayerState<AFQPlayerState_InGame>())
            {
                PS->SetSoulType(PlayerInfomation.mSoulType);
                PS->SetArmourType(EArmourType::End);
                PS->SetSoulGauge(0);
            }
            
            // UI 생성
            PC->CreateUI();
            UE_LOG(LogTemp, Log, TEXT("[FQGameMode_InGame %d] Added Player %d"), __LINE__, ControllerId);
        }
    }
}

FTransform AFQGameMode_InGame::GetMainCameraTransform() const
{
    if (mMainCamera)
    {
        return mMainCamera->GetTransform();
    }
    return FTransform();
}

void AFQGameMode_InGame::StartGame()
{
    UGameplayStatics::OpenLevel(this, "TitleScreen");
}

void AFQGameMode_InGame::ExitGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void AFQGameMode_InGame::MoveButton(const FInputActionValue& Value, int32 ControllerId)
{
	// UI On/Off
	if (mGamePauseUI && mGamePauseUI->GetVisibility() == ESlateVisibility::Visible)
	{
		FVector2D Direction = Value.Get<FVector2D>();
		if (Direction.X >= 0.8f)
		{
			mGamePauseUI->WidgetInput(EWidgetInputType::Up);
		}
		if (Direction.X <= -0.8f)
		{
			mGamePauseUI->WidgetInput(EWidgetInputType::Down);
		}
		if (Direction.Y >= 0.8f)
		{
			mGamePauseUI->WidgetInput(EWidgetInputType::Right);
		}
		if (Direction.Y <= -0.8f)
		{
			mGamePauseUI->WidgetInput(EWidgetInputType::Left);
		}
	}
}

void AFQGameMode_InGame::CancelInteraction(int32 ControllerId)
{
	// UI On/Off
	if (mGamePauseUI && mGamePauseUI->GetVisibility() == ESlateVisibility::Visible)
	{
		mGamePauseUI->WidgetInput(EWidgetInputType::Cancel);
	}
}

void AFQGameMode_InGame::SelectInteraction(int32 ControllerId)
{
    // UI On/Off
	if (mGamePauseUI && mGamePauseUI->GetVisibility() == ESlateVisibility::Visible)
	{
		mGamePauseUI->WidgetInput(EWidgetInputType::Select);
	}
}

void AFQGameMode_InGame::MenuInteraction()
{
    // UI On/Off
    IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
    if (MyGameInstance)
    {
        MyGameInstance->SetOnWidget(!MyGameInstance->GetOnWidget());

        if (mGamePauseUI && MyGameInstance->GetOnWidget())
        {
            mGamePauseUI->InitUI();
            mGamePauseUI->SetVisibility(ESlateVisibility::Visible);
            GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f); // 시간 정지
        }
        else if (mGamePauseUI)
        {
            mGamePauseUI->SetVisibility(ESlateVisibility::Hidden);
            GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f); // 정상 속도 복귀
        }
    }
}
