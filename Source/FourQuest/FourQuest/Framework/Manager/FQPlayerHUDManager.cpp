// Fill out your copyright notice in the Description page of Project Settings.

#include "FQPlayerHUDManager.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

#include "FourQuest\FourQuest\Framework\InGame\FQ_GameInstance_InGame.h"

AFQPlayerHUDManager::AFQPlayerHUDManager()
{
	// UPROPERTY로 노출된 UUserWidget* 클래스를 지정
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetRef(TEXT("/Game/Blueprints/HUD/WBP_PlayerHUDWidget.WBP_PlayerHUDWidget_C"));
	if (HUDWidgetRef.Class)
	{
		mHUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetRef.Class);
		if (mHUDWidget)
		{
			mHUDWidget->AddToViewport(); // 화면에 위젯 추가
			UE_LOG(LogTemp, Log, TEXT("[AFQPlayerHUDManager %d] Create HUD Widget And AddViewport"), __LINE__);
		}
	}
}

void AFQPlayerHUDManager::AddPlayerController(APlayerController* NewPlayer, UUserWidget* PlayerHUDWidget)
{
	if (mHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerHUDManager %d] Is Not Valid HUDWidget!!"), __LINE__);
		return;
	}

	UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(mHUDWidget->GetWidgetFromName(TEXT("PlayerHUD")));
	if (mHUDWidget == nullptr || HorizontalBox == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerHUDManager %d] Is Not Valid HUDWidget Or HorizontalBox!!"), __LINE__);
		return;
	}

	if (UWorld* World = NewPlayer->GetWorld())
	{
		// 플레이어 전용 HUD 생성
		if (PlayerHUDWidget)
		{
			// HorizontalBox에 추가
			UHorizontalBoxSlot* NewSlot = HorizontalBox->AddChildToHorizontalBox(PlayerHUDWidget);
			if (NewSlot)
			{
				// Fill 방식 설정 (Weight = 1)
				NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

				// 정렬 설정 (가로, 세로 모두 중앙 정렬)
				NewSlot->SetHorizontalAlignment(HAlign_Center);
				NewSlot->SetVerticalAlignment(VAlign_Center);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerHUDManager %d] Is Not Valid PlayerHUDWidget!!"), __LINE__);
			return;
		}

		mPlayerHUDs.Add(PlayerHUDWidget);
		UE_LOG(LogTemp, Log, TEXT("[AFQPlayerHUDManager %d] HorizontalBox Add PlayerHUD"), __LINE__);
	}
}