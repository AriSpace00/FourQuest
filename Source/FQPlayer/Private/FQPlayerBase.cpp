// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayerBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "FQGameCore/Player/FQPlayerStateInterface.h"

// Sets default values
AFQPlayerBase::AFQPlayerBase()
{
	mMoveDirection = FVector::Zero();

	// Pawn
	// 컨트롤러에 의한 카메라 회전 방지
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// 캡슐 컴포넌트 크기 및 콜리전 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCollision"));
	
	// Input
	mDefaultSpeed = 200.0f;
	
	// Mesh
	// 캐릭터 메쉬 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->bReceivesDecals = false;

	// Effect
	mSoulEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SoulEffect"));
	mSoulEffect->SetupAttachment(RootComponent);
	mSoulEffect->SetAutoActivate(false);

	mHitState = EHitState::None;
	mMoveState = EMoveState::CanMove;

	mbIsPressedX = false;
	mbIsPressedA = false;

	Tags.Add(FName("Player"));
}

void AFQPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &AFQPlayerBase::Move);
}

void AFQPlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

float AFQPlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.f)
	{
		return 0.f;
	}

	Hit();
	ProcessHitInterrupt();

	IFQPlayerStateInterface* PSInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PSInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AFQPlayerBase::TakeDamage] Current Hp : %d"), PSInterface->GetHp());
		PSInterface->SetHp(PSInterface->GetHp() - ActualDamage);
		UE_LOG(LogTemp, Warning, TEXT("[AFQPlayerBase::TakeDamage] Next Hp : %d"), PSInterface->GetHp());
	}
	
	return ActualDamage;
}

void AFQPlayerBase::ApplyDamageToTarget(float DamageAmount, AActor* Target)
{
	UGameplayStatics::ApplyDamage(Target, DamageAmount, GetController(), this, UDamageType::StaticClass());
}

bool AFQPlayerBase::IsHit()
{
	if (mHitState == EHitState::HitReacting)
	{
		return true;
	}

	return false;
}

void AFQPlayerBase::TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength)
{
	if (!Target)
	{
		return;
	}

	if (Target != this)
	{
		return;
	}

	LaunchCharacter(Direction * Strength, true, true);
}

void AFQPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context 적용
	SetInputMappingContext();

	// Character Movement 기본 설정 적용
	SetMovement();

	// Animation MontageEnded 바인딩
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}
	AnimInstance->OnMontageEnded.AddDynamic(this, &AFQPlayerBase::OnHitAnimEnded);

	// 영혼 타입에 따른 이펙트 설정
	IFQPlayerStateInterface* PSInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PSInterface)
	{
		switch(PSInterface->GetSoulType())
		{
		case ESoulType::Sword :
		{
			if (mKnightEffectSystem)
			{
				mSoulEffect->SetAsset(mKnightEffectSystem);
			}
		}
		break;
		case ESoulType::Staff :
		{
			if (mMageEffectSystem)
			{
				mSoulEffect->SetAsset(mMageEffectSystem);
			}
		}
		break;
		case ESoulType::Axe :
		{
			if (mWarriorEffectSystem)
			{
				mSoulEffect->SetAsset(mWarriorEffectSystem);
			}
		}
		break;
		case ESoulType::Bow :
		{
			if (mArcherEffectSystem)
			{
				mSoulEffect->SetAsset(mArcherEffectSystem);
			}
		}
		break;
		}

		mSoulEffect->Activate();
	}
}

void AFQPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector2D NormalizedVector = MovementVector.GetSafeNormal();
	mMoveDirection = FVector(NormalizedVector.X, NormalizedVector.Y, 0.0f);

	ProcessInputMovement();

	if (!CanMove())
	{
		return;
	}

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AFQPlayerBase::SetInputMappingContext()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mDefaultMappingContext, 0);
	}
}

void AFQPlayerBase::SetMovement()
{
	// Movement
	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 3000.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
}

void AFQPlayerBase::Hit()
{
	mHitState = EHitState::HitReacting;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(mHitAnim);
}

void AFQPlayerBase::OnHitAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == mHitAnim)
	{
		mHitState = EHitState::None;
	}
}
