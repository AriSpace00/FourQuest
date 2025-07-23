// Fill out your copyright notice in the Description page of Project Settings.


#include "FQGameMode_SoulSelect.h"

#include "Blueprint/UserWidget.h"

#include "FQUI\UI\FQSoulSelectScreenWidget.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerController_InGame.h"
#include "FourQuest\FourQuest\Framework\InGame\FQPlayerState_InGame.h"
#include "FourQuest\FourQuest\Framework\InGame\FQ_GameInstance_InGame.h"

#include "Kismet\GameplayStatics.h"

AFQGameMode_SoulSelect::AFQGameMode_SoulSelect()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 함수가 실행되기 위한 bool형 타입 변수
}

void AFQGameMode_SoulSelect::StartPlay()
{
    Super::StartPlay();

    CreatePlayer();
}

void AFQGameMode_SoulSelect::BeginPlay()
{
	Super::BeginPlay();

	if (mSoulSelectWidgetClass)
	{
		mSoulSelectWidgetHandle = CreateWidget<UFQSoulSelectScreenWidget>(GetWorld(), mSoulSelectWidgetClass);
		mSoulSelectWidgetHandle->SetOwningActor(this);
		mSoulSelectWidgetHandle->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] mTitleWidgetClass is nullptr"), __LINE__);
	}
}

void AFQGameMode_SoulSelect::Tick(float DeltaTime)
{
    if (mSoulSelectWidgetHandle->GetStartNextLevel())
    {
        if (mTargetLevelName.IsNone())
        {
            UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] TargetLevelName is not set!"), __LINE__);
            return;
        }

        for (int32 i = 0; i < 4; i++)
        {
            FQ_InGameSetting::FQLocalMultiPlayerInfomation PlayerInfomation;
            PlayerInfomation.bSpawnLocalPlayer = mPlayerStates[i] == EPlayerStateType::SoulComplate ? true : false;
            PlayerInfomation.mSoulType = mPlayerSoulTypes[i];

            UFQ_GameInstance_InGame* MyGameInstance = Cast<UFQ_GameInstance_InGame>(GetGameInstance());
            if (MyGameInstance)
            {
                MyGameInstance->SavePlayerInfomation(i, PlayerInfomation);
            }
        }

        UGameplayStatics::OpenLevel(this, mTargetLevelName);
        UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] StartGame: Opening level '%s'"), __LINE__, *mTargetLevelName.ToString());
    }
}

void AFQGameMode_SoulSelect::CreatePlayer()
{
    UGameInstance* MyGameInstance = GetGameInstance();
    if (!MyGameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] MyGameInstance is nullptr!!"), __LINE__);
        return;
    }

    for (int32 i = 0; i < 4; i++)
    {
        int32 ControllerId = i;

        // CreateLocalPlayer는 자동으로 PlayerController를 생성하고 해당 입력을 매핑함
        FString ErrorMessage;
        ULocalPlayer* NewPlayer = MyGameInstance->CreateLocalPlayer(ControllerId, ErrorMessage, true);
        if (NewPlayer)
        {
            // 플레이어 스테이트 클래스에 소울 타입 지정
            AFQPlayerController_InGame* PC = Cast<AFQPlayerController_InGame>(NewPlayer->GetPlayerController(GetWorld()));
            if (!PC)
            {
                UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] AFQPlayerController_InGame Is nullptr!!"), __LINE__);
                return;
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] Create AFQPlayerController_InGame Id : %d!!"), __LINE__, ControllerId);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[AFQGameMode_SoulSelect %d] Faild Create NewPlayer Id : %d!!"), __LINE__, ControllerId);
        }

        mPlayerStates.Add(i, EPlayerStateType::NoSelect);
        mPlayerSoulTypes.Add(i, ESoulType::Sword);
    }
}

void AFQGameMode_SoulSelect::MoveButton(const FInputActionValue& Value, int32 ControllerId)
{
    FVector2D Direction = Value.Get<FVector2D>();

    if (Direction.Y >= 0.8f && mPlayerStates[ControllerId] == EPlayerStateType::SoulSelect)     // Right
    {
        mPlayerSoulTypes[ControllerId] = mPlayerSoulTypes[ControllerId] == ESoulType::Bow ? ESoulType::Bow : (ESoulType)((int8)(mPlayerSoulTypes[ControllerId]) + 1);
        mSoulSelectWidgetHandle->WidgetInput(mPlayerSoulTypes[ControllerId], ControllerId);
    }
    if (Direction.Y <= -0.8f && mPlayerStates[ControllerId] == EPlayerStateType::SoulSelect)    // Left
    {
        mPlayerSoulTypes[ControllerId] = mPlayerSoulTypes[ControllerId] == ESoulType::Sword ? ESoulType::Sword : (ESoulType)((int8)(mPlayerSoulTypes[ControllerId]) - 1);
        mSoulSelectWidgetHandle->WidgetInput(mPlayerSoulTypes[ControllerId], ControllerId);
    }
}

void AFQGameMode_SoulSelect::CancelInteraction(int32 ControllerId)
{
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] CancelInteraction Function Call : PlayerController Id(%d)"), __LINE__, ControllerId);
    mPlayerStates[ControllerId] = mPlayerStates[ControllerId] == EPlayerStateType::NoSelect ? EPlayerStateType::NoSelect : (EPlayerStateType)((int8)(mPlayerStates[ControllerId]) - 1);

    mSoulSelectWidgetHandle->UpdatePlayerState(mPlayerStates[ControllerId], mPlayerSoulTypes[ControllerId], ControllerId);
}

void AFQGameMode_SoulSelect::SelectInteraction(int32 ControllerId)
{
	UE_LOG(LogTemp, Log, TEXT("[AFQGameMode_SoulSelect %d] SelectInteraction Function Call : PlayerController Id(%d)"), __LINE__, ControllerId);
    mPlayerStates[ControllerId] = mPlayerStates[ControllerId] == EPlayerStateType::SoulComplate ? EPlayerStateType::SoulComplate : (EPlayerStateType)((int8)(mPlayerStates[ControllerId]) + 1);

    mSoulSelectWidgetHandle->UpdatePlayerState(mPlayerStates[ControllerId], mPlayerSoulTypes[ControllerId], ControllerId);
}

void AFQGameMode_SoulSelect::MenuInteraction()
{
}

void AFQGameMode_SoulSelect::StartGame()
{
}

void AFQGameMode_SoulSelect::ExitGame()
{
}