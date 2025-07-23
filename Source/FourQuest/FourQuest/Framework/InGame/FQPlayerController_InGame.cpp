// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerController_InGame.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kismet\GameplayStatics.h"

#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQGameCore\GameMode\FQGameModeInterface.h"
#include "FQGameMode_InGame.h"
#include "FQPlayerState_InGame.h"
#include "FourQuest\FourQuest\Framework\Manager\FQPlayerHUDManager.h"
#include "FourQuest\FourQuest\Actor\FQMainCenterCamera.h"
#include "FQSoul\public\FQSoulBase.h"
#include "FQPlayer\Public\FQPlayerBase.h"
#include "FQUI/Player/FQPlayerHUDWidget.h"
#include "FQGameCore\Player\FQPlayerInputDataAsset.h"
#include "FourQuest\FourQuest\Actor\FQPlayerUIActor.h"
#include "FQGameCore/Quest/FQQuestSystem.h"
#include "FQGameCore\GameInstance\FQGameInstanceInterface.h"
#include "FQGameCore\GameMode\FQGameModeUIInputInterface.h"

AFQPlayerController_InGame::AFQPlayerController_InGame()
{
	// UI 생성 등록
	static ConstructorHelpers::FClassFinder<UFQPlayerHUDWidget> ArmourWidgetRef(TEXT("/Game/Blueprints/HUD/WBP_PlayerWidget.WBP_PlayerWidget_C"));
	if (ArmourWidgetRef.Class)
	{
		mPlayerHUDWidget = CreateWidget<UFQPlayerHUDWidget>(GetWorld(), ArmourWidgetRef.Class);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Faild Load mPlayerHUDWidgetClass"), __LINE__);
	}
}

void AFQPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();

}

void AFQPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 로그 확인
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		int32 ControllerId = LocalPlayer->GetControllerId();
		UE_LOG(LogTemp, Log, TEXT("[AFQPlayerController_InGame %d] SetupInputComponent Function Call : PlayerController Id(%d)"), __LINE__, ControllerId);
	}

	// Local Player Subsystem에 MappingContext 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mPlayerInputDataAsset->mDefaultMappingContext, 0);
	}

	// EnhancedInputComponent 사용
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] InputComponent Is nullptr"), __LINE__);
		return;
	}

	// 입력 함수 등록
	if (mPlayerInputDataAsset)
	{
		EnhancedInput->BindAction(mPlayerInputDataAsset->mAButtonAction, ETriggerEvent::Started, this, &AFQPlayerController_InGame::HandlePickButton);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mBButtonAction, ETriggerEvent::Started, this, &AFQPlayerController_InGame::HandleCancelButton);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Triggered, this, &AFQPlayerController_InGame::HandleMoveTriggered);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Completed, this, &AFQPlayerController_InGame::HandleMoveCompleted);
		EnhancedInput->BindAction(mPlayerInputDataAsset->mMenuButtonAction, ETriggerEvent::Started, this, &AFQPlayerController_InGame::HandleMenuButton);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] mPlayerInputDataAsset Is Not Vaild!!"), __LINE__);
	}
}



void AFQPlayerController_InGame::CreateUI()
{
	UpdateHUDSetting();
	SpawnPlayerUIActor();
}

void AFQPlayerController_InGame::SpawnPlayerUIActor()
{
	if (mPlayerOverheadUIActor == nullptr && GetWorld())
	{
		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;              // 소유자 지정
		SpawnParams.Instigator = GetPawn();    // 필요시
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 위치는 플레이어 캐릭터 기준
		FVector UISpawnLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;

		mPlayerOverheadUIActor = GetWorld()->SpawnActor<AFQPlayerUIActor>(
			AFQPlayerUIActor::StaticClass(),
			UISpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (mPlayerOverheadUIActor)
		{
			// 컨트롤러가 붙은 Pawn을 따라가도록 설정
			mPlayerOverheadUIActor->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepWorldTransform);
		}
	}

	// 로그 확인
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		int32 ControllerId = LocalPlayer->GetControllerId();

		// 플레이어 스테이트 클래스 확인
		AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
		if (FQPlayerState && mPlayerOverheadUIActor)
		{
			mPlayerOverheadUIActor->UpdatePlayerNumber(ControllerId, FQPlayerState->GetSoulType());

			// 델리게이트 등록
			FQPlayerState->mPlayerHpDelegate.AddLambda([&](float CurrentHpValue)
				{
					if (mPlayerOverheadUIActor)
					{
						mPlayerOverheadUIActor->UpdatePlayerHp(CurrentHpValue);
					}
				});
			FQPlayerState->mArmourChangeDelegate.AddLambda([&](EArmourType NewArmourType)
				{
					if (mPlayerOverheadUIActor)
					{
						mPlayerOverheadUIActor->UpdateArmourType(NewArmourType);
					}
				});
			FQPlayerState->mPlayerDeadDelegate.AddLambda([&](bool bIsDead)
				{
					if (bIsDead)
					{
						ChangeToSoul();
						mPlayerOverheadUIActor->UpdateArmourType(EArmourType::End);
					}
				});
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] FQPlayerState Or mPlayerOverheadUIActor Is Not Valid!!"), __LINE__, ControllerId);
		}
	}
}

void AFQPlayerController_InGame::UpdateHUDSetting()
{
	// 플레이어 스테이트 클래스 확인
	AFQPlayerState_InGame* FQPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (FQPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild FQPlayerState_InGame"), __LINE__);
		return;
	}

	// HUD Widget 콜백 함수 Binding
	FQPlayerState->mArmourChangeDelegate.AddLambda([&](EArmourType NewArmourType)
		{
			if (mPlayerHUDWidget)
			{
				mPlayerHUDWidget->UpdateArmourSkill(NewArmourType);
			}
		});
	FQPlayerState->mSoulGaugeDelegate.AddLambda([&](float CurrentSoulGauge)
		{
			if (mPlayerHUDWidget)
			{
				mPlayerHUDWidget->UpdateSoulGauge(CurrentSoulGauge);
			}
		});
	FQPlayerState->mPlayerHpDelegate.AddLambda([&](float HpValue)
		{
			if (mPlayerHUDWidget)
			{
				mPlayerHUDWidget->UpdateHpValue(HpValue);
			}
		});

	// UFQPlayerHUDWidget에 소유 오브젝트 등록
	if (mPlayerHUDWidget)
	{
		mPlayerHUDWidget->SetOwningActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild UFQPlayerHUDWidget"), __LINE__);
	}

	// 게임 모드에 플레이어 컨트롤러 등록
	AFQGameMode_InGame* FQGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
	if (FQGameMode)
	{
		FQGameMode->GetPlayerHUDManager()->AddPlayerController(this, mPlayerHUDWidget);

		AFQMainCenterCamera* Camera = FQGameMode->GetMainCamera();
		if (Camera)
		{
			SetViewTargetWithBlend(Camera);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Not Find CameraActor!!"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Is Not Vaild AFQGameMode_InGame!!"), __LINE__);
	}
}

void AFQPlayerController_InGame::ChangeToArmour(EArmourType InArmourType)
{
	APawn* PlayerCharacter = GetPawn();
	FTransform SpawnTransform = FTransform(PlayerCharacter->GetActorTransform().GetLocation());
	if (PlayerCharacter)
	{
		PlayerCharacter->Destroy();
	}

	// 사용할 블루프린트 클래스 로딩
	TSubclassOf<AFQPlayerBase> CharacterBPClass = mPlayerArmourCharacterClasses[InArmourType];
	if (CharacterBPClass)
	{
		CreatePlayerCharacterByClass(CharacterBPClass, SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Failed to load character blueprint class"), __LINE__);
	}

	// 플레이어 스테이트 세팅
	AFQPlayerState_InGame* MyPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (MyPlayerState)
	{
		MyPlayerState->SetArmourType(InArmourType);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] AFQPlayerState_InGame Is Not Vaild!!"), __LINE__);
	}
}

void AFQPlayerController_InGame::ChangeToSoul()
{
	// 현재 플레이어 캐릭터 삭제
	APawn* PlayerCharacter = GetPawn();
	FTransform SpawnTransform = FTransform(PlayerCharacter->GetActorTransform().GetLocation());
	if (PlayerCharacter)
	{
		PlayerCharacter->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Character Is nullptr"), __LINE__);
		return;
	}

	// 플레이어 스테이트 세팅
	AFQPlayerState_InGame* MyPlayerState = GetPlayerState<AFQPlayerState_InGame>();
	if (MyPlayerState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] AFQPlayerState_InGame Is Not Vaild!!"), __LINE__);
		return;
	}

	MyPlayerState->SetArmourType(EArmourType::End);
	ESoulType PlayerSoulType = MyPlayerState->GetSoulType();

	// 사용할 플레이어 캐릭터 클래스 탐색 후 생성
	TSubclassOf<AFQSoulBase> CharacterBPClass = mPlayerSoulCharacterClasses[PlayerSoulType];
	if (CharacterBPClass)
	{
		CreateSoulCharacterByClass(CharacterBPClass, SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame %d] Failed to load character blueprint class"), __LINE__);
	}
}



void AFQPlayerController_InGame::CreatePlayerCharacterByClass(TSubclassOf<AFQPlayerBase> CharacterClass, const FTransform& SpawnTransform)
{
	if (!CharacterClass) return;

	// 갑옷 캐릭터 생성
	AFQPlayerBase* NewCharacter = GetWorld()->SpawnActorDeferred<AFQPlayerBase>(CharacterClass, SpawnTransform);
	if (NewCharacter)
	{
		Possess(NewCharacter);							// 플레이어 캐릭터 등록
		NewCharacter->FinishSpawning(SpawnTransform);	// 캐릭터 초기화 끝 BeginPlay 함수 실행

		// 카메라 등록
		AFQGameMode_InGame* InGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
		if (InGameMode)
		{
			SetViewTargetWithBlend(InGameMode->GetMainCamera());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame &d] AFQGameMode_InGame Is Not Vailed!!"), __LINE__);
		}

		// Head UI 캐릭터 등록
		if (mPlayerOverheadUIActor)
		{
			// 컨트롤러가 붙은 Pawn을 따라가도록 설정
			mPlayerOverheadUIActor->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AFQPlayerController_InGame::CreateSoulCharacterByClass(TSubclassOf<class AFQSoulBase> CharacterClass, const FTransform& SpawnTransform)
{
	if (!CharacterClass) return;

	// 영혼 캐릭터 생성
	AFQSoulBase* NewCharacter = GetWorld()->SpawnActorDeferred<AFQSoulBase>(CharacterClass, SpawnTransform);
	if (NewCharacter)
	{
		NewCharacter->FinishSpawning(SpawnTransform);	// 캐릭터 초기화 끝 BeginPlay 함수 실행
		Possess(NewCharacter);							// 플레이어 캐릭터 등록

		// 카메라 등록
		AFQGameMode_InGame* InGameMode = Cast<AFQGameMode_InGame>(GetWorld()->GetAuthGameMode());
		if (InGameMode)
		{
			SetViewTargetWithBlend(InGameMode->GetMainCamera());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQPlayerController_InGame &d] AFQGameMode_InGame Is Not Vailed!!"), __LINE__);
		}

		// Head UI 캐릭터 등록
		if (mPlayerOverheadUIActor)
		{
			// 컨트롤러가 붙은 Pawn을 따라가도록 설정
			mPlayerOverheadUIActor->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}


void AFQPlayerController_InGame::HandlePickButton()
{
	// 입력 버튼
	IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
		{
			int32 ControllerId = LocalPlayer->GetControllerId();
			UE_LOG(LogTemp, Log, TEXT("[AFQPlayerController_InGame %d] HandlePickButton Function Call : Controller Id(%d)"), __LINE__, ControllerId);
			MyGameMode->SelectInteraction(ControllerId);
		}
	}
	else
	{
		return;
	}

	// 현재 UI가 켜져 있는가
	IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
	if (MyGameInstance)
	{
		// 선택 소리 재생
		if (mSelectButtonSound && MyGameInstance->GetOnWidget())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				mSelectButtonSound,                 // USoundBase*
				FVector()							// FVector 위치
			);
		}
	}
}

void AFQPlayerController_InGame::HandleCancelButton()
{
	// 취소 버튼
	IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
		{
			int32 ControllerId = LocalPlayer->GetControllerId();
			UE_LOG(LogTemp, Log, TEXT("[AFQPlayerController_InGame %d] HandleCancelButton Function Call : Controller Id(%d)"), __LINE__, ControllerId);
			MyGameMode->CancelInteraction(ControllerId);
		}
	}
	else
	{
		return;
	}

	// 현재 UI가 켜져 있는가
	IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
	if (MyGameInstance)
	{
		// 선택 소리 재생
		if (mCancelButtonSound && MyGameInstance->GetOnWidget())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				mCancelButtonSound,					// USoundBase*
				FVector()							// FVector 위치
			);
		}
	}
}

void AFQPlayerController_InGame::HandleMenuButton()
{
	IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		MyGameMode->MenuInteraction();
	}

	// 현재 UI가 켜져 있는가
	IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
	if (MyGameInstance)
	{
		// 선택 소리 재생
		if (mSelectButtonSound && MyGameInstance->GetOnWidget())
		{ 
			UGameplayStatics::PlaySoundAtLocation(
				this,
				mSelectButtonSound,					// USoundBase*
				FVector()							// FVector 위치
			);
		}
	}
}

void AFQPlayerController_InGame::HandleMoveTriggered(const FInputActionValue& Value)
{
	// 방향 업데이트
	mMoveDirection = Value.Get<FVector2D>();

	if (mbIsMoveKeyHeld)
	{
		return;
	}
	if (FMath::Abs(mMoveDirection.X) >= 0.8f || FMath::Abs(mMoveDirection.Y) >= 0.8f)
	{
		mbIsMoveKeyHeld = true;
		DoMove();
	}

	// 반복 타이머 설정 (무조건 실행)
	GetWorldTimerManager().SetTimer(
		mRepeatMoveTimerHandle,
		this,
		&AFQPlayerController_InGame::DoMove,
		0.2f,
		true,
		1.0f
	);
}

void AFQPlayerController_InGame::HandleMoveCompleted(const FInputActionValue& Value)
{
	mbIsMoveKeyHeld = false;
	GetWorldTimerManager().ClearTimer(mRepeatMoveTimerHandle);
}

void AFQPlayerController_InGame::DoMove()
{
	if (FMath::Abs(mMoveDirection.X) < 0.8f && FMath::Abs(mMoveDirection.Y) < 0.8f)
	{
		return;
	}
	
	// 이동
	IFQGameModeUIInputInterface* MyGameMode = Cast<IFQGameModeUIInputInterface>(GetWorld()->GetAuthGameMode());
	if (MyGameMode)
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
		{
			int32 ControllerId = LocalPlayer->GetControllerId();
			MyGameMode->MoveButton(mMoveDirection, ControllerId);
		}
	}
	else
	{
		return;
	}

	// 현재 UI가 켜져 있는가
	IFQGameInstanceInterface* MyGameInstance = GetGameInstance<IFQGameInstanceInterface>();
	if (MyGameInstance)
	{
		// 선택 소리 재생
		if (mMoveButtonSound && MyGameInstance->GetOnWidget())
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				mMoveButtonSound,					// USoundBase*
				FVector()							// FVector 위치
			);
		}
	}
}
