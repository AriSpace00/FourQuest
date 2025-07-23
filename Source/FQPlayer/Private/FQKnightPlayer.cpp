// Fill out your copyright notice in the Description page of Project Settings.


#include "FQKnightPlayer.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "FQGameCore/Player/FQPlayerAttackableInterface.h"
#include "FQGameCore/Player/FQPlayerStateInterface.h"
#include "FQPlayer/Public/FQKnightDataAsset.h"

AFQKnightPlayer::AFQKnightPlayer()
{
	// Bash 
	mbCanBash = true;
	mbIsBashing = false;
	mBashVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BashVolume"));
	mBashVolume->SetupAttachment(RootComponent);

	// Knight
	mSwordAttackState = EKnightSwordAttackState::None;
	mSwordAttackComboState = EComboState::None;
	mSwordAttackVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordAttackVolume"));
	mSwordAttackVolume->SetupAttachment(RootComponent);

	// Shield
	mShieldState = EKnightShieldState::None;
	mbIsShielding = false;
	mShieldMoveAngle = 0.0f;
	mShieldRotation = FRotator::ZeroRotator;
	mShieldVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldVolume"));
	mShieldVolume->SetupAttachment(RootComponent);

	// Effect
	mShieldEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldEffect"));
	mShieldEffect->SetAutoActivate(false);
	mShieldEffect->SetupAttachment(RootComponent);

	// Weapon Mesh
	mSwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	mSwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	mShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Sound
	mBashHitAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BashAttackAudio"));
	mBashHitAudio->SetupAttachment(RootComponent);
	mBashHitAudio->SetAutoActivate(false);

	mShieldAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ShieldAudio"));
	mShieldAudio->SetupAttachment(RootComponent);
	mShieldAudio->SetAutoActivate(false);
}

void AFQKnightPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!mKnightDataAsset)
	{
		return;
	}

	if (mbIsBashing && mKnightDataAsset->mBashCurve)
	{
		mBashElapsedTime += DeltaSeconds;
		float NormalizedTime = FMath::Clamp(mBashElapsedTime / mKnightDataAsset->mBashDuration, 0.0f, 1.0f);

		mBashDirection = GetLastMovementInputVector().GetSafeNormal();
		if (mBashDirection.IsZero())
		{
			mBashDirection = GetActorForwardVector();
		}

		AddMovementInput(mBashDirection, mKnightDataAsset->mBashCurve->GetFloatValue(NormalizedTime));

		if (mBashElapsedTime >= mKnightDataAsset->mBashDuration)
		{
			EndBash();
		}
	}

	if (mShieldState == EKnightShieldState::Shield)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, mShieldRotation, DeltaSeconds, mKnightDataAsset->mShieldRotationSpeed);
		SetActorRotation(NewRotation);
	}
}

void AFQKnightPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Bash (A)
	EnhancedInputComponent->BindAction(mBashAction, ETriggerEvent::Started, this, &AFQKnightPlayer::Bash);

	// Sword Attack (X)
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Started, this, &AFQKnightPlayer::StartSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Triggered, this, &AFQKnightPlayer::PressedSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Completed, this, &AFQKnightPlayer::EndSwordAttack);
	EnhancedInputComponent->BindAction(mSwordAttackAction, ETriggerEvent::Canceled, this, &AFQKnightPlayer::EndSwordAttack);

	// Shield (Right Stick)
	EnhancedInputComponent->BindAction(mShieldAction, ETriggerEvent::Started, this, &AFQKnightPlayer::StartShieldMove);
	EnhancedInputComponent->BindAction(mShieldAction, ETriggerEvent::Triggered, this, &AFQKnightPlayer::PressedShieldMove);
	EnhancedInputComponent->BindAction(mShieldAction, ETriggerEvent::Completed, this, &AFQKnightPlayer::EndShieldMove);
}

void AFQKnightPlayer::EnableAttackVolume()
{
	if (!mSwordAttackVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[EnableSwordAttackVolume]"));

	mSwordAttackVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFQKnightPlayer::DisableAttackVolume()
{
	if (!mSwordAttackVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[DisableSwordAttackVolume]"));

	mSwordAttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQKnightPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Knight IMC 바인딩
	SetInputMappingContext();

	// Knight 에셋에서 기본 속도 설정
	if (mKnightDataAsset)
	{
		mDefaultSpeed = mKnightDataAsset->mDefaultSpeed;
		GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	}
	
	// Collision BeginOverlap 바인딩
	if (mBashVolume)
	{
		mBashVolume->OnComponentBeginOverlap.AddDynamic(this, &AFQKnightPlayer::OnBashVolumeBeginOverlap);
	}
	
	if (mShieldVolume)
	{
		mShieldVolume->OnComponentBeginOverlap.AddDynamic(this, &AFQKnightPlayer::OnShieldVolumeBeginOverlap);
	}
	
	// Animation MontageEnded 바인딩
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AFQKnightPlayer::OnKnightAnimMontageEnded);
	}
	
	// PlayerState 최대 체력 설정
	IFQPlayerStateInterface* PSInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PSInterface)
	{
		PSInterface->SetMaxHp(mKnightDataAsset->mMaxHp);
	}

	// Weapon Mesh
	if (mSwordMesh)
	{
		mSwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Knight_Sword")));
	}

	if (mShieldMesh)
	{
		mShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Knight_Shield")));
	}

	// Sound
	mBashHitAudio->RegisterComponent();
	mShieldAudio->RegisterComponent();
}

void AFQKnightPlayer::SetInputMappingContext()
{
	Super::SetInputMappingContext();

	if (!mKnightMappingContext)
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mKnightMappingContext, 1);
	}
}

bool AFQKnightPlayer::CanMove()
{
	if (mMoveState == EMoveState::CannotMove)
	{
		return false;
	}

	return true;
}

void AFQKnightPlayer::ProcessInputMovement()
{
}

void AFQKnightPlayer::ProcessNextSection()
{
	// X버튼을 꾹 누르고 있을 때
	if (!mbIsPressedX)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[ProcessNextSection] 콤보 상태로 전환"));

	mSwordAttackComboState = EComboState::Combo;
}

void AFQKnightPlayer::ProcessHitInterrupt()
{
	if (mShieldState == EKnightShieldState::Shield)
	{
		mShieldState = EKnightShieldState::CoolDown;

		DisableShieldVolume();

		GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { mShieldState = EKnightShieldState::None; });
		GetWorld()->GetTimerManager().SetTimer(mShieldCoolTimer, TimerDel, mKnightDataAsset->mShieldCoolTime, false);
	}

	mMoveState = EMoveState::CanMove;
}

void AFQKnightPlayer::ProcessSwordAttack()
{
	TArray<AActor*> OverlappedActors;
	mSwordAttackVolume->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		if (!Actor)
		{
			return;
		}

		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (!RootComp)
		{
			return;
		}

		if (RootComp == GetRootComponent())
		{
			continue;
		}

		if (mKnightDataAsset->mSwordAttackableTypes.IsEmpty())
		{
			return;
		}

		if (!mKnightDataAsset->mSwordAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
		{
			continue;
		}

		ApplySwordAttackDamage(Actor);
	}
}

void AFQKnightPlayer::EnableBashVolume()
{
	if (!mBashVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[EnableBashVolume]"));

	mBashVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 이미 Bash Volume에 오버랩된 액터 밀어내기
	CheckBashVolume();
}

void AFQKnightPlayer::DisableBashVolume()
{
	if (!mBashVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[DisableBashVolume]"));

	mBashVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQKnightPlayer::EnableShieldVolume()
{
	if (!mShieldVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[EnableShieldVolume]"));

	mShieldVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	mShieldEffect->Activate();

	// 이미 Shield Volume에 오버랩된 액터 밀어내기
	CheckShiedVolume();

	if (!mShieldAudio->IsPlaying())
	{
		mShieldAudio->Play();
	}
}

void AFQKnightPlayer::DisableShieldVolume()
{
	if (!mShieldVolume)
	{
		return;
	}

	// Debug
	UE_LOG(LogTemp, Log, TEXT("[DisableShieldVolume]"));

	mShieldVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mShieldEffect->ResetSystem();
	mShieldEffect->Deactivate();

	mShieldAudio->Stop();
}

float AFQKnightPlayer::GetShieldMoveAngle()
{
	return mShieldMoveAngle;
}

bool AFQKnightPlayer::IsHoldShield()
{
	if (mShieldState == EKnightShieldState::Shield)
	{
		return true;
	}
	return false;
}

void AFQKnightPlayer::Bash()
{
	// R스킬 실행 중일 때
	if (!(mShieldState == EKnightShieldState::None || mShieldState == EKnightShieldState::CoolDown))
	{
		return;
	}

	if (!mbCanBash)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (!AnimInstance->Montage_IsPlaying(mBashAnim))
	{
		AnimInstance->Montage_Play(mBashAnim);
	}

	StartBash();
}

void AFQKnightPlayer::StartSwordAttack()
{
	// R스킬 실행 중일 때
	if (mShieldState == EKnightShieldState::Shield)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	if (mSwordAttackComboState == EComboState::None)
	{
		// 공격 입력이 처음으로 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] 공격 입력이 처음으로 들어왔을 때"));

		if (mSwordAttackState == EKnightSwordAttackState::CoolDown)
		{
			return;
		}

		// 공격 상태 설정
		mSwordAttackState = EKnightSwordAttackState::Attack1;

		// 애니메이션 재생
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}
		AnimInstance->Montage_Play(mSwordAttackAnim1);

		mMoveState = EMoveState::CannotMove;

		// 콤보 가능 상태 설정
		mSwordAttackComboState = EComboState::CanBeCombo;
		GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);
		GetWorld()->GetTimerManager().SetTimer(mSwordAttackComboTimer, this, &AFQKnightPlayer::ResetSwordAttackCombo, mKnightDataAsset->mSwordAttackWaitTime1, false);
	}
	else if (mSwordAttackComboState == EComboState::CanBeCombo)
	{ 
		// 입력 대기 시간 내에 입력이 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] 입력 대기 시간 내에 입력이 들어왔을 때"));

		// 콤보 가능 상태 설정
		mSwordAttackComboState = EComboState::Combo;

		switch (mSwordAttackState)
		{
		case EKnightSwordAttackState::Attack1:
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mSwordAttackAnim1))
			{
				return;
			}

			mSwordAttackState = EKnightSwordAttackState::Attack2;

			AnimInstance->Montage_Play(mSwordAttackAnim2);

			mMoveState = EMoveState::CannotMove;

			GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mSwordAttackComboTimer, this, &AFQKnightPlayer::ResetSwordAttackCombo,mKnightDataAsset->mSwordAttackWaitTime2, false);

			UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] Attack2 Anim 재생"));
		}
		break;
		case EKnightSwordAttackState::Attack2:
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mSwordAttackAnim2))
			{
				return;
			}

			mSwordAttackState = EKnightSwordAttackState::Attack3;

			AnimInstance->Montage_Play(mSwordAttackAnim3);

			GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);

			mMoveState = EMoveState::CannotMove;

			UE_LOG(LogTemp, Log, TEXT("[StartSwordAttack] Attack3 Anim 재생"));
		}
		break;
		}
	}
}

void AFQKnightPlayer::EndSwordAttack()
{
	if (mbIsPressedX)
	{
		mbIsPressedX = false;
	}
}

void AFQKnightPlayer::StartShieldMove()
{
	// A스킬 실행 중일때
	if (mbIsBashing)
	{
		return;
	}

	if (mShieldState == EKnightShieldState::CoolDown)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (!mShieldUpAnim)
	{
		return;
	}

	if (!AnimInstance->Montage_IsPlaying(mShieldUpAnim))
	{
		AnimInstance->Montage_Play(mShieldUpAnim);
	}

	EnableShieldVolume();

	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed * (mKnightDataAsset->mShieldWalkRatio / 100.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	mShieldState = EKnightShieldState::Shield;
}

void AFQKnightPlayer::PressedShieldMove(const FInputActionValue& Value)
{
	if (mShieldState != EKnightShieldState::Shield)
	{
		return;
	}

	// 입력 X축 반전
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector2D NormalizedVector = MovementVector.GetSafeNormal();
	NormalizedVector.X *= -1.0f;

	// 플레이어 회전 각도 설정 
	float PlayerAngle = FMath::Atan2(NormalizedVector.Y, NormalizedVector.X) * (180.0f / PI);
	mShieldRotation = FRotator(0.0f, PlayerAngle, 0.0f);

	//UE_LOG(LogTemp, Log, TEXT("[ShieldMove] InputAngle : %f"), PlayerAngle);

	// 플레이어가 바라보는 방향 벡터
	FVector LookDir = FVector(NormalizedVector.X, NormalizedVector.Y, 0.0f);

	// 사잇각 크기
	float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LookDir, mMoveDirection)));

	// 방향성 판단 (BlendSpace)
	FVector Cross = FVector::CrossProduct(LookDir, mMoveDirection);
	mShieldMoveAngle = AngleDegrees * FMath::Sign(Cross.Z);

	//UE_LOG(LogTemp, Log, TEXT("[ShieldMove] mShieldMoveAngle : %f"), mShieldMoveAngle);
}

void AFQKnightPlayer::EndShieldMove()
{
	if (mShieldState != EKnightShieldState::Shield)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (!mShieldDownAnim)
	{
		return;
	}

	if (!AnimInstance->Montage_IsPlaying(mShieldDownAnim))
	{
		AnimInstance->Montage_Play(mShieldDownAnim);
	}

	mShieldState = EKnightShieldState::CoolDown;

	DisableShieldVolume();

	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this]() { mShieldState = EKnightShieldState::None; });
	GetWorld()->GetTimerManager().SetTimer(mShieldCoolTimer, TimerDel, mKnightDataAsset->mShieldCoolTime, false);
}

void AFQKnightPlayer::CheckShiedVolume()
{
	TArray<AActor*> OverlappedActors;
	mShieldVolume->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		if (!Actor)
		{
			return;
		}

		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (!RootComp)
		{
			return;
		}

		if (RootComp == GetRootComponent())
		{
			continue;
		}

		if (mKnightDataAsset->mShieldAttackableTypes.IsEmpty())
		{
			return;
		}

		if (!mKnightDataAsset->mShieldAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
		{
			continue;
		}

		ApplyDamageToTarget(mKnightDataAsset->mShieldDamage, Actor);
		ApplyPush(Actor, mKnightDataAsset->mShieldStrength);

		if (mShieldHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mShieldHitEffect, Actor->GetActorLocation(), Actor->GetActorRotation(), FVector(1.0f), true, true);
		}	
	}
}

void AFQKnightPlayer::OnKnightAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == mSwordAttackAnim1)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mSwordAttackComboState == EComboState::Combo)
		{
			mSwordAttackState = EKnightSwordAttackState::Attack2;

			mSwordAttackComboState = EComboState::CanBeCombo;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mSwordAttackAnim2))
			{
				return;
			}

			AnimInstance->Montage_Play(mSwordAttackAnim2);
			
			GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mSwordAttackComboTimer, this, &AFQKnightPlayer::ResetSwordAttackCombo, mKnightDataAsset->mSwordAttackWaitTime2, false);

			UE_LOG(LogTemp, Log, TEXT("[OnAnimMontageEnded] Attack2 Anim 재생"));
		}
		else
		{
			mMoveState = EMoveState::CanMove;
		}

	}
	else if (Montage == mSwordAttackAnim2)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mSwordAttackComboState == EComboState::Combo)
		{
			mSwordAttackState = EKnightSwordAttackState::Attack3;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mSwordAttackAnim3))
			{
				return;
			}

			AnimInstance->Montage_Play(mSwordAttackAnim3);

			GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);

			UE_LOG(LogTemp, Log, TEXT("[OnAnimMontageEnded] Attack3 Anim 재생"));
		}
		else
		{
			mMoveState = EMoveState::CanMove;
		}
	}
	else if (Montage == mSwordAttackAnim3)
	{
		UE_LOG(LogTemp, Log, TEXT("[OnAnimMontageEnded] 콤보 리셋"));

		GetWorld()->GetTimerManager().ClearTimer(mSwordAttackComboTimer);
		ResetSwordAttackCombo();

		mMoveState = EMoveState::CanMove;
	}
}

void AFQKnightPlayer::OnShieldVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (!RootComp)
	{
		return;
	}

	if (RootComp == GetRootComponent())
	{
		return;
	}

	if (mKnightDataAsset->mShieldAttackableTypes.IsEmpty())
	{
		return;
	}

	if (!mKnightDataAsset->mShieldAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
	{
		return;
	}

	ApplyDamageToTarget(mKnightDataAsset->mShieldDamage, OtherActor);
	ApplyPush(OtherActor, mKnightDataAsset->mShieldStrength);

	if (mShieldHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mShieldHitEffect, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(1.0f), true, true);
	}
}

void AFQKnightPlayer::StartBash()
{
	if (mbIsBashing)
	{
		return;
	}

	mbCanBash = false;
	mbIsBashing = true;
	mBashElapsedTime = 0.0f;

	GetCharacterMovement()->MaxWalkSpeed = mKnightDataAsset->mBashSpeed;
	GetCharacterMovement()->MaxAcceleration = 200000.0f;

	GetWorld()->GetTimerManager().SetTimer(mBashCoolTimer, this, &AFQKnightPlayer::ResetBash, mKnightDataAsset->mBashCoolTime, false);
}

void AFQKnightPlayer::EndBash()
{
	mbIsBashing = false;

	GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void AFQKnightPlayer::ResetBash()
{
	mbCanBash = true;

	mHitActors.Empty();
}

void AFQKnightPlayer::CheckBashVolume()
{
	TArray<AActor*> OverlappedActors;
	mBashVolume->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		if (!Actor)
		{
			return;
		}

		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (!RootComp)
		{
			return;
		}

		if (mHitActors.Contains(Actor))
		{
			return;
		}

		if (RootComp == GetRootComponent())
		{
			continue;
		}

		if (mKnightDataAsset->mBashAttackableTypes.IsEmpty())
		{
			return;
		}

		if (!mKnightDataAsset->mBashAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
		{
			continue;
		}

		ApplyDamageToTarget(mKnightDataAsset->mBashDamage, Actor);
		ApplyPush(Actor, mKnightDataAsset->mBashStrength);
		mHitActors.Add(Actor);

		if (!mBashHitAudio->IsPlaying())
		{
			mBashHitAudio->Play();
		}
	}
}

void AFQKnightPlayer::OnBashVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (!RootComp)
	{
		return;
	}

	if (mHitActors.Contains(OtherActor))
	{
		return;
	}

	if (RootComp == GetRootComponent())
	{
		return;
	}

	if (mKnightDataAsset->mBashAttackableTypes.IsEmpty())
	{
		return;
	}

	if (!mKnightDataAsset->mBashAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
	{
		return;
	}

	ApplyDamageToTarget(mKnightDataAsset->mBashDamage, OtherActor);
	ApplyPush(OtherActor, mKnightDataAsset->mBashStrength);
	mHitActors.Add(OtherActor);

	if (!mBashHitAudio->IsPlaying())
	{
		mBashHitAudio->Play();
	}
}

void AFQKnightPlayer::ResetSwordAttackCombo()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetSwordAttackCombo] ResetCombo"));

	mSwordAttackComboState = EComboState::None;

	mSwordAttackState = EKnightSwordAttackState::CoolDown;

	GetWorld()->GetTimerManager().ClearTimer(mSwordAttackCoolTimer);
	GetWorld()->GetTimerManager().SetTimer(mSwordAttackCoolTimer, this, &AFQKnightPlayer::ResetSwordAttackCoolDown, mKnightDataAsset->mSwordAttackCoolTime, false);
}

void AFQKnightPlayer::ResetSwordAttackCoolDown()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetSwordAttackCoolDown] ResetCoolDown"));

	mSwordAttackState = EKnightSwordAttackState::None;
}

void AFQKnightPlayer::PressedSwordAttack()
{
	mbIsPressedX = true;
}

void AFQKnightPlayer::ApplySwordAttackDamage(AActor* AttackableActor)
{
	UE_LOG(LogTemp, Log, TEXT("[ApplyDamage] Current Type : %s"), *UEnum::GetValueAsString(mSwordAttackState));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	switch (mSwordAttackState)
	{
	case EKnightSwordAttackState::Attack1:
	{
		ApplyDamageToTarget(mKnightDataAsset->mSwordAttackDamage1, AttackableActor);

		if (mSwordAttackAudio1)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, mSwordAttackAudio1, GetActorLocation());
		}

		if (mSwordHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mSwordHitEffect, AttackableActor->GetActorLocation(), AttackableActor->GetActorRotation(), FVector(1.0f), true, true);
		}
	}
	break;
	case EKnightSwordAttackState::Attack2:
	{
		ApplyDamageToTarget(mKnightDataAsset->mSwordAttackDamage2, AttackableActor);

		if (mSwordAttackAudio2)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, mSwordAttackAudio2, GetActorLocation());
		}

		if (mSwordHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mSwordHitEffect, AttackableActor->GetActorLocation(), AttackableActor->GetActorRotation(), FVector(1.0f), true, true);
		}
	}
	break;
	case EKnightSwordAttackState::Attack3:
	{
		ApplyDamageToTarget(mKnightDataAsset->mSwordAttackDamage3, AttackableActor);
		ApplyPush(AttackableActor, mKnightDataAsset->mSwordAttackStrength);

		if (mSwordAttackAudio3)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, mSwordAttackAudio3, GetActorLocation());
		}

		if (mShieldHitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mShieldHitEffect, AttackableActor->GetActorLocation(), AttackableActor->GetActorRotation(), FVector(1.0f), true, true);
		}
	}
	break;
	}
}

void AFQKnightPlayer::ApplyPush(AActor* AttackableActor, float Strength)
{
	IFQPlayerAttackableInterface* PlayerAttackableInterface = Cast<IFQPlayerAttackableInterface>(AttackableActor);
	if (!PlayerAttackableInterface)
	{
		return;
	}

	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.f;
	Forward.Normalize();

	PlayerAttackableInterface->TakePushByPlayer(AttackableActor, Forward, Strength);
}
