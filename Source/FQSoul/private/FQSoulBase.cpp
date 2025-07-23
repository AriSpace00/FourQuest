// Copyright (Your Project)

#include "FQSoulBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "Blueprint\UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "FQSoulDataAsset.h"
#include "FQGameCore/Armour/FQArmourInterface.h"
#include "FQGameCore\Player\FQPlayerControllerInterface.h"
#include "FQGameCore\Player\FQPlayerStateInterface.h"
#include "FQGameCore\Player\FQPlayerInputDataAsset.h"
#include "FQUI\FQWidgetComponent.h"
#include "FQUI/Soul/FQSoulGaugeWidget.h"
#include "Kismet\GameplayStatics.h"

AFQSoulBase::AFQSoulBase()
{
	// Movement State
	mDashDirection = FVector::ZeroVector;
	mbIsDashing = false;

	// Armour State
	mArmourChangeTimer = 0.f;

	// Capsule Setup
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AFQSoulBase::OnOverlapEnd);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SoulCollision"));

	// Movement Setup
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AFQSoulBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;

	if (mEquipEffectSystem)
	{
		mEquipEffectSystem->ConditionalPostLoad();  // 최소한의 보장
		mEquipEffectSystem->AddToRoot();            // GC 방지용 (선택)
	}
}

void AFQSoulBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Dash Logic
	mDashCoolTimer -= DeltaTime;
	if (mSoulDataAsset && mbIsDashing)
	{
		AddMovementInput(mDashDirection, mSoulDataAsset->mDashSpeed * DeltaTime);
		mDashTimer -= DeltaTime;
		if (mDashTimer <= 0.0f)
		{
			GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = 2048.f;
			mDashCoolTimer = mSoulDataAsset->mDashCoolTime;
			mbIsDashing = false;
		}
	}

	IFQArmourInterface* Nearest = CheckNearArmour();
	IFQArmourInterface* CurrentArmourInterface = Cast<IFQArmourInterface>(mCurrentArmour);
	// 가장 가까운 갑옷이 다르면 취소
	if (CurrentArmourInterface != Nearest)
	{
		if (CurrentArmourInterface) CurrentArmourInterface->SetNearestArmour(false);
		if (Nearest) Nearest->SetNearestArmour(true);
		mCurrentArmour = Cast<UObject>(Nearest);
		return;
	}
}

void AFQSoulBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(mPlayerInputDataAsset->mDefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] UEnhancedInputLocalPlayerSubsystem Is Not Vaild!!"), __LINE__);
		}
	}

	if (mPlayerInputDataAsset)
	{
		UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		Input->BindAction(mPlayerInputDataAsset->mLeftStickAction, ETriggerEvent::Triggered, this, &AFQSoulBase::Move);
		Input->BindAction(mPlayerInputDataAsset->mBButtonAction, ETriggerEvent::Started, this, &AFQSoulBase::CheckArmour);
		//Input->BindAction(mPlayerInputDataAsset->mYButtonAction, ETriggerEvent::Triggered, this, &AFQSoulBase::AddSoulGauge);
		Input->BindAction(mPlayerInputDataAsset->mAButtonAction, ETriggerEvent::Triggered, this, &AFQSoulBase::StartDash);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SetupPlayerInputComponent %d] mPlayerInputDataAsset Is Not Vaild!!"), __LINE__);
	}
}

void AFQSoulBase::Move(const FInputActionValue& Value)
{
	if (mbIsDashing) return;

	FVector2D InputVec = Value.Get<FVector2D>();
	float SizeSq = InputVec.SquaredLength();
	float MoveScale = SizeSq > 1.0f ? 1.0f : FMath::Sqrt(SizeSq);

	if (SizeSq > 1.0f) InputVec.Normalize();

	FVector MoveDir = FVector(InputVec.X, InputVec.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDir).Rotator());
	AddMovementInput(MoveDir, MoveScale);
}

void AFQSoulBase::CheckArmour()
{
	// 갑옷이 0개이거나 버튼을 안눌렀으면 중지
	if (mArmours.Num() == 0 || !mCurrentArmour) return;

	// 이펙트 시작
	if (mEquipEffectSystem)
	{
		mEquipEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			mEquipEffectSystem,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true, true, ENCPoolMethod::None, true
		);

		if (mEquipEffectComponent)
		{
			// 연출 종료시 호출될 함수 바인딩
			mEquipEffectComponent->OnSystemFinished.AddDynamic(this, &AFQSoulBase::OnEquipEffectFinished);
		}

		APlayerController* MyPC = Cast<APlayerController>(GetController());
		if (MyPC)
		{
			DisableInput(MyPC); // 캐릭터에 대한 입력만 막음
		}

		IFQPlayerStateInterface* MyPlayerState = GetPlayerState<IFQPlayerStateInterface>();
		if (!MyPlayerState)
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] PlayerStateInterface가 유효하지 않습니다!"), __LINE__);
			return;
		}

		IFQArmourInterface* CurrentArmourInterface = Cast<IFQArmourInterface>(mCurrentArmour);
		if (CurrentArmourInterface)
		{
			MyPlayerState->SetArmourType(CurrentArmourInterface->GetArmourType());
			CurrentArmourInterface->PickArmour();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] CurrentArmourInterface가 유효하지 않습니다!"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] mEquipEffectSystem is Null!!"), __LINE__);
	}
}

void AFQSoulBase::StartDash()
{
	if (mSoulDataAsset && !mbIsDashing && mDashCoolTimer <= 0)
	{
		mbIsDashing = true;
		mDashTimer = mSoulDataAsset->mDashDuration;
		GetCharacterMovement()->MaxWalkSpeed = mSoulDataAsset->mDashSpeed;
		GetCharacterMovement()->MaxAcceleration = mSoulDataAsset->mDashSpeed * 2;

		mDashDirection = GetLastMovementInputVector().IsZero() ? GetActorForwardVector() : GetLastMovementInputVector().GetSafeNormal();
	}
}

void AFQSoulBase::AddSoulGauge()
{
	IFQPlayerStateInterface* PlayerStateInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PlayerStateInterface)
	{
		PlayerStateInterface->AddSoulGauge(5);
		PlayerStateInterface->AddHp(-3);
	}
}

IFQArmourInterface* AFQSoulBase::CheckNearArmour()
{
	IFQArmourInterface* Closest = nullptr;
	float MinDistSq = FLT_MAX;
	FVector Origin = GetActorLocation();

	for (const auto& Entry : mArmours)
	{
		IFQArmourInterface* Armour = Cast<IFQArmourInterface>(Entry.Value);
		if (!Armour) continue;

		float DistSq = FVector::DistSquared(Origin, Armour->GetActorTransform().GetLocation());
		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			Closest = Armour;
		}
	}

	return Closest;
}

void AFQSoulBase::OnEquipEffectFinished(UNiagaraComponent* PSystem)
{
	IFQPlayerStateInterface* MyPlayerState = GetPlayerState<IFQPlayerStateInterface>();
	if (!MyPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQSoulBase %d] PlayerStateInterface가 유효하지 않습니다!"), __LINE__);
		return;
	}

	// 갑옷 타입 확인한 후 갑옷 입기
	EArmourType Type = MyPlayerState->GetArmourType();
	UE_LOG(LogTemp, Log, TEXT("[AFQSoulBase %d] Effect Finished Callback Function Call : %s"), __LINE__, *UEnum::GetValueAsString(Type));

	IFQPlayerControllerInterface* PlayerController = Cast<IFQPlayerControllerInterface>(GetController());
	if (PlayerController)
	{
		PlayerController->ChangeToArmour(Type);
	}
}


// Overlap
void AFQSoulBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (IFQArmourInterface* Armour = Cast<IFQArmourInterface>(OtherActor))
	{
		mArmours.Add(OtherActor->GetName(), OtherActor);
	}
}

void AFQSoulBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IFQArmourInterface* Armour = Cast<IFQArmourInterface>(OtherActor))
	{
		Armour->SetNearestArmour(false);
		mArmours.Remove(OtherActor->GetName());
	}
}


// IFQSoulCharacterInterface
FTransform AFQSoulBase::GetActorTransform() const
{
	return GetTransform();
}