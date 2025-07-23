// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMagePlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

#include "FQGameCore/Player/FQPlayerAttackableInterface.h"
#include "FQGameCore/Player/FQPlayerStateInterface.h"

#include "FQPlayer/Public/FQMageDataAsset.h" 
#include "FQPlayer/Public/FQMageProjectile.h" 
#include "FQPlayer/Public/FQMageCircle.h" 
#include "FQPlayer/Public/FQMageStaff.h"

AFQMagePlayer::AFQMagePlayer()
{
	mProjectileAttackVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileAttackVolume"));
	mProjectileAttackVolume->SetupAttachment(RootComponent);

	mLookAtDirection = FVector::ZeroVector;
	mLookAtRotation = FRotator::ZeroRotator;

	mProjectileAttackState = EMageProjectileAttackState::None;
	mProjectileAttackComboState = EComboState::None;
	mExplosionState = EMageExplosionState::None;
	mLaserState = EMageLaserState::None;

	mbLaserCoolDown = false;
}

void AFQMagePlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!mMageDataAsset)
	{
		return;
	}

	switch (mProjectileAttackState)
	{
	case EMageProjectileAttackState::Attack1:
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}

		if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
		{
			SetActorRotation(mLookAtRotation);
		}
	}
	break;
	case EMageProjectileAttackState::Attack2:
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}

		if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
		{
			SetActorRotation(mLookAtRotation);
		}
	}
	break;
	}

	if (mLaserState == EMageLaserState::Enabled)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, mLaserRotation, DeltaSeconds, mMageDataAsset->mLaserRotationSpeed);
		SetActorRotation(NewRotation);

		UpdateLaser();
	}
}

void AFQMagePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Projectile Attack (X)
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Started, this, &AFQMagePlayer::StartProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Triggered, this, &AFQMagePlayer::PressedProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Canceled, this, &AFQMagePlayer::EndProjectileAttack);
	EnhancedInputComponent->BindAction(mProjectileAttackAction, ETriggerEvent::Completed, this, &AFQMagePlayer::EndProjectileAttack);

	// Explosion (A)
	EnhancedInputComponent->BindAction(mExplosionAction, ETriggerEvent::Started, this, &AFQMagePlayer::StartExplosion);
	EnhancedInputComponent->BindAction(mExplosionAction, ETriggerEvent::Triggered, this, &AFQMagePlayer::PressedExplosion);
	EnhancedInputComponent->BindAction(mExplosionAction, ETriggerEvent::Completed, this, &AFQMagePlayer::EndExplosion);

	// Laser (R)
	EnhancedInputComponent->BindAction(mLaserAction, ETriggerEvent::Started, this, &AFQMagePlayer::StartLaser);
	EnhancedInputComponent->BindAction(mLaserAction, ETriggerEvent::Triggered, this, &AFQMagePlayer::PressedLaser);
	EnhancedInputComponent->BindAction(mLaserAction, ETriggerEvent::Completed, this, &AFQMagePlayer::EndLaser);
}

void AFQMagePlayer::ProcessNextSection()
{
	if (!mbIsPressedX)
	{
		return;
	}

	if (mProjectileAttackComboState == EComboState::CanBeCombo)
	{
		mProjectileAttackComboState = EComboState::Combo;
	}
}

bool AFQMagePlayer::IsEnabledExplosionCircle()
{
	if (mExplosionState == EMageExplosionState::Enabled)
	{
		return true;
	}

	return false;
}

bool AFQMagePlayer::IsEnabledLaser()
{
	if (mLaserState == EMageLaserState::Enabled)
	{
		return true;
	}

	return false;
}

void AFQMagePlayer::EnableAttackVolume()
{
	if (!mProjectileAttackVolume)
	{
		return;
	}

	mProjectileAttackVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CheckProjectileAttackVolume();
}

void AFQMagePlayer::DisableAttackVolume()
{
	if (!mProjectileAttackVolume)
	{
		return;
	}

	mProjectileAttackVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQMagePlayer::BeginPlay()
{
	Super::BeginPlay();

	// Mage IMC 바인딩
	SetInputMappingContext();

	// Mage 에셋에서 기본 속도 설정
	if (mMageDataAsset)
	{
		mDefaultSpeed = mMageDataAsset->mDefaultSpeed;
		GetCharacterMovement()->MaxWalkSpeed = mDefaultSpeed;
	}

	// Animation MontageEnded 바인딩
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AFQMagePlayer::OnMageAnimMontageEnded);
	}
	
	// Staff Actor 
	if (mStaffClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		mStaff = GetWorld()->SpawnActor<AFQMageStaff>(mStaffClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		mStaff->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("Mage_Staff")));
	}

	// PlayerState 최대 체력 설정
	IFQPlayerStateInterface* PSInterface = Cast<IFQPlayerStateInterface>(GetPlayerState());
	if (PSInterface)
	{
		PSInterface->SetMaxHp(mMageDataAsset->mMaxHp);
	}
}

void AFQMagePlayer::SetInputMappingContext()
{
	Super::SetInputMappingContext();

	if (!mMageMappingContext)
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(mMageMappingContext, 1);
	}
}

bool AFQMagePlayer::CanMove()
{
	if (mMoveState == EMoveState::CannotMove)
	{
		return false;
	}

	return true;
}

void AFQMagePlayer::ProcessInputMovement()
{
	if (!mExplosionCircle)
	{
		return;
	}

	// 마법진 위치 
	FVector CurrentLocation = mExplosionCircle->GetActorLocation();
	FVector DeltaMove = mMoveDirection * mMageDataAsset->mExplosionCircleSpeed * GetWorld()->GetDeltaSeconds();
	FVector NewLocation = CurrentLocation + DeltaMove;

	float Distance = FVector::Dist(GetActorLocation(), NewLocation);
	if (Distance >= mMageDataAsset->mMaxDistance)
	{
		return;
	}

	mExplosionCircle->SetActorLocation(NewLocation);

	// 마법진 크기 
	if (Distance > mMageDataAsset->mMinDistance)
	{
		float ExcessDistance = Distance - mMageDataAsset->mMinDistance;
		float MaxExcess = mMageDataAsset->mMaxDistance - mMageDataAsset->mMinDistance;

		float Alpha = FMath::Clamp(ExcessDistance / MaxExcess, 0.0f, 1.0f);
		float NewScaleFactor = FMath::Lerp(mMageDataAsset->mMaxScale, mMageDataAsset->mMinScale, Alpha);

		mExplosionCircle->SetScale(NewScaleFactor);
	}
	else
	{
		mExplosionCircle->SetScale(mMageDataAsset->mMaxScale);
	}
}

void AFQMagePlayer::OnMageAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Projectile Attack
	if (Montage == mProjectileAttackAnim1)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mProjectileAttackComboState == EComboState::Combo)
		{
			mProjectileAttackState = EMageProjectileAttackState::Attack2;
			mProjectileAttackComboState = EComboState::CanBeCombo;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
			{
				return;
			}

			AnimInstance->Montage_Play(mProjectileAttackAnim2);

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
		}
		else
		{
			mMoveState = EMoveState::CanMove;
		}
	}
	else if (Montage == mProjectileAttackAnim2)
	{
		if (mHitState == EHitState::HitReacting)
		{
			return;
		}

		if (mProjectileAttackComboState == EComboState::Combo)
		{
			mProjectileAttackState = EMageProjectileAttackState::Attack1;
			mProjectileAttackComboState = EComboState::CanBeCombo;

			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 몽타주 중복 실행 방지
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
			{
				return;
			}

			AnimInstance->Montage_Play(mProjectileAttackAnim1);

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime2, false);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			ResetProjectileAttackCombo();

			mMoveState = EMoveState::CanMove;
		}
	}

	if (Montage == mExplosionEndAnim)
	{
		// 쿨타임 설정
		mExplosionState = EMageExplosionState::CoolDown;

		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { mExplosionState = EMageExplosionState::None; });
		GetWorld()->GetTimerManager().SetTimer(mExplosionCoolTimer, TimerDel, mMageDataAsset->mExplosionCoolTime, false);

		// 이펙트 재생
		mExplosionCircle->ActivateEffect();
		mExplosionCircle = nullptr;

		mMoveState = EMoveState::CanMove;
	}

	// Laser
	if (Montage == mLaserStartAnim && !bInterrupted)
	{
		mLaserState = EMageLaserState::Enabled;

		// 최소 지속 시간 타이머 설정
		GetWorld()->GetTimerManager().ClearTimer(mLaserDurationTimer);

		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { mbLaserCoolDown = true; });
		GetWorld()->GetTimerManager().SetTimer(mLaserDurationTimer, TimerDel, mMageDataAsset->mLaserMinDuration, false);
	}

	if (Montage == mLaserEndAnim)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		mStaff->DeactivateLaserEffect();

		mMoveState = EMoveState::CanMove;
	}
}

void AFQMagePlayer::StartProjectileAttack()
{
	// R스킬 실행 중일 때
	if (!(mLaserState == EMageLaserState::None || mLaserState == EMageLaserState::CoolDown))
	{
		return;
	}

	// A스킬 실행 중일 때
	if (!(mExplosionState == EMageExplosionState::None || mExplosionState == EMageExplosionState::CoolDown))
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	if (mProjectileAttackComboState == EComboState::None)
	{
		// 공격 입력이 처음으로 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartProjectileAttack] 공격 입력이 처음으로 들어왔을 때"));

		if (mProjectileAttackState == EMageProjectileAttackState::CoolDown)
		{
			return;
		}

		// 공격 상태 설정
		mProjectileAttackState = EMageProjectileAttackState::Attack1;

		// 애니메이션 재생
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}
		AnimInstance->Montage_Play(mProjectileAttackAnim1);

		mMoveState = EMoveState::CannotMove;

		// 콤보 가능 상태 설정
		mProjectileAttackComboState = EComboState::CanBeCombo;
		GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
		GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
	}
	else if (mProjectileAttackComboState == EComboState::CanBeCombo)
	{
		// 입력 대기 시간 내에 입력이 들어왔을 때
		UE_LOG(LogTemp, Log, TEXT("[StartProjectileAttack] 입력 대기 시간 내에 입력이 들어왔을 때"));

		// 콤보 가능 상태 설정
		mProjectileAttackComboState = EComboState::Combo;

		switch (mProjectileAttackState)
		{
		case EMageProjectileAttackState::Attack1:
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim1))
			{
				return;
			}

			mProjectileAttackState = EMageProjectileAttackState::Attack2;

			AnimInstance->Montage_Play(mProjectileAttackAnim2);

			mMoveState = EMoveState::CannotMove;

			GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackComboTimer);
			GetWorld()->GetTimerManager().SetTimer(mProjectileAttackComboTimer, this, &AFQMagePlayer::ResetProjectileAttackCombo, mMageDataAsset->mProjectileAttackWaitTime1, false);
		}
		break;
		case EMageProjectileAttackState::Attack2:
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!AnimInstance)
			{
				return;
			}

			// 이전 애니메이션이 끝났을 때
			if (AnimInstance->Montage_IsPlaying(mProjectileAttackAnim2))
			{
				return;
			}

			mProjectileAttackState = EMageProjectileAttackState::Attack1;

			AnimInstance->Montage_Play(mProjectileAttackAnim1);

			mMoveState = EMoveState::CannotMove;
		}
		break;
		}
	}
}

void AFQMagePlayer::PressedProjectileAttack()
{
	mbIsPressedX = true;
}

void AFQMagePlayer::EndProjectileAttack()
{
	if (mbIsPressedX)
	{
		mbIsPressedX = false;
	}
}

void AFQMagePlayer::ResetProjectileAttackCombo()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetProjectileAttackCombo] ResetCombo"));

	mProjectileAttackComboState = EComboState::None;

	mProjectileAttackState = EMageProjectileAttackState::CoolDown;

	GetWorld()->GetTimerManager().ClearTimer(mProjectileAttackCoolTimer);
	GetWorld()->GetTimerManager().SetTimer(mProjectileAttackCoolTimer, this, &AFQMagePlayer::ResetProjectileAttackCoolDown, mMageDataAsset->mProjectileAttackCoolTime, false);
}

void AFQMagePlayer::ResetProjectileAttackCoolDown()
{
	UE_LOG(LogTemp, Log, TEXT("[ResetProjectileAttackCoolDown] ResetCoolDown"));

	mProjectileAttackState = EMageProjectileAttackState::None;
}

void AFQMagePlayer::StartExplosion()
{
	// R스킬 실행 중일 때
	if (!(mLaserState == EMageLaserState::None || mLaserState == EMageLaserState::CoolDown))
	{
		return;
	}

	if (mExplosionState != EMageExplosionState::None)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	if (!mExplosionCircleClass)
	{
		return;
	}

	mbIsPressedA = true;

	// 애니메이션 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(mExplosionStartAnim))
	{
		return;
	}

	AnimInstance->Montage_Play(mExplosionStartAnim);

	// 마법진 생성
	if (!mExplosionCircleClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	mExplosionCircle = GetWorld()->SpawnActor<AFQMageCircle>(mExplosionCircleClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	// 상태 설정
	if (mExplosionCircle)
	{
		mExplosionState = EMageExplosionState::Enabled;
		mExplosionCircle->PlayCircleAudio();
	}

	mMoveState = EMoveState::CannotMove;
}

void AFQMagePlayer::PressedExplosion()
{
	if (mExplosionState != EMageExplosionState::Enabled)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		// 피격 상태일 때 즉시 마법진 공격 실행
		ProcessExplosion();
	}
}

void AFQMagePlayer::EndExplosion()
{
	if (mExplosionState != EMageExplosionState::Enabled)
	{
		return;
	}

	ProcessExplosion();
}

void AFQMagePlayer::ProcessExplosion()
{
	mExplosionCircle->PlayExplosionAudio();
	mExplosionCircle->StopCircleAudio();

	TArray<AActor*> OverlappedActors;
	mExplosionCircle->mVolume->GetOverlappingActors(OverlappedActors);
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

		if (mMageDataAsset->mExplosionAttackableTypes.IsEmpty())
		{
			return;
		}

		if (!mMageDataAsset->mExplosionAttackableTypes.Contains(RootComp->GetCollisionObjectType()))
		{
			continue;
		}

		ApplyDamageToTarget(mMageDataAsset->mExplosionDamageAmount, Actor);
	}

	// 애니메이션 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(mExplosionEndAnim))
	{
		return;
	}

	AnimInstance->Montage_Play(mExplosionEndAnim);

	mExplosionState = EMageExplosionState::Destroy;
	mbIsPressedA = false;
}

void AFQMagePlayer::StartLaser(const FInputActionValue& Value)
{
	// A스킬 실행 중일 때
	if (!(mExplosionState == EMageExplosionState::None || mExplosionState == EMageExplosionState::CoolDown))
	{ 
		return;
	}

	if (mLaserState == EMageLaserState::CoolDown)
	{
		return;
	}

	if (mHitState == EHitState::HitReacting)
	{
		return;
	}

	// 상태 설정
	mLaserState = EMageLaserState::InputReceived;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 사운드 재생
	mStaff->PlayLaserAudio();
}

void AFQMagePlayer::PressedLaser(const FInputActionValue& Value)
{
	// 레이저 방향 설정
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector2D NormalizedVector = MovementVector.GetSafeNormal();
	NormalizedVector.X *= -1.0f;

	float PlayerAngle = FMath::Atan2(NormalizedVector.Y, NormalizedVector.X) * (180.0f / PI);
	mLaserRotation = FRotator(0.0f, PlayerAngle, 0.0f);

	// 시전 애니메이션 재생
	if (mLaserState == EMageLaserState::InputReceived)
	{
		SetActorRotation(mLaserRotation);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return;
		}

		if (AnimInstance->Montage_IsPlaying(mLaserStartAnim))
		{
			return;
		}

		AnimInstance->Montage_Play(mLaserStartAnim);

		mLaserState = EMageLaserState::Preparing;

		mMoveState = EMoveState::CannotMove;
	}
}

void AFQMagePlayer::EndLaser()
{
	if (!(mLaserState == EMageLaserState::Preparing || mLaserState == EMageLaserState::Enabled))
	{
		return;
	}

	// 애니메이션 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(mLaserEndAnim))
	{
		return;
	}

	AnimInstance->Montage_Play(mLaserEndAnim);

	mStaff->DeactivateLaserEffect();
	mStaff->DeactivateHitEffect();
	mStaff->StopLaserAudio();

	mCurrentLaserTarget = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(mLaserDamageTimer);
	GetWorld()->GetTimerManager().ClearTimer(mLaserDurationTimer);

	if (mbLaserCoolDown)
	{
		mbLaserCoolDown = false;
		mLaserState = EMageLaserState::CoolDown;

		GetWorld()->GetTimerManager().ClearTimer(mLaserCoolTimer);

		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { mLaserState = EMageLaserState::None; });
		GetWorld()->GetTimerManager().SetTimer(mLaserCoolTimer, TimerDel, mMageDataAsset->mLaserCoolTime, false);
	}
	else
	{
		mLaserState = EMageLaserState::None;
	}
}

void AFQMagePlayer::ApplyLaserDamage()
{
	if (mCurrentLaserTarget)
	{
		ApplyDamageToTarget(mMageDataAsset->mLaserDamageAmount, mCurrentLaserTarget);
	}
}

void AFQMagePlayer::UpdateLaser()
{
	FHitResult HitResult;
	FVector TraceStart = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FVector TraceEnd = TraceStart + GetActorForwardVector() * 500.0f;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;

	if (mMageDataAsset->mLaserAttackableTypes.IsEmpty())
	{
		return;
	}

	for (ECollisionChannel Channel : mMageDataAsset->mLaserAttackableTypes)
	{
		ObjectQueryParams.AddObjectTypesToQuery(Channel);
	}

	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		TraceStart,
		TraceEnd,
		ObjectQueryParams,
		TraceParams
	);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 3.0f);

	if (bHit && HitResult.GetActor())
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		mStaff->ActivateLaserEffect(ImpactPoint);
		mStaff->ActivateHitEffect(ImpactPoint);

		if (mCurrentLaserTarget != HitResult.GetActor())
		{
			mCurrentLaserTarget = HitResult.GetActor();

			ApplyLaserDamage();
			GetWorld()->GetTimerManager().ClearTimer(mLaserDamageTimer);
			GetWorld()->GetTimerManager().SetTimer(mLaserDamageTimer, this, &AFQMagePlayer::ApplyLaserDamage, mMageDataAsset->mLaserDamageTime, true);
		}
	}
	else
	{
		FVector EffectEnd = mStaff->GetLaserEffectLocation();
		EffectEnd.Z += 5.0f;
		mStaff->ActivateLaserEffect(EffectEnd);
		mStaff->DeactivateHitEffect();

		mCurrentLaserTarget = nullptr;

		GetWorld()->GetTimerManager().ClearTimer(mLaserDamageTimer);
	}
}

bool AFQMagePlayer::ApplyPush(AActor* AttackableActor)
{
	if (mMageDataAsset->mProjectileAttackableTypes.IsEmpty())
	{
		return false;
	}

	if (!mMageDataAsset->mProjectileAttackableTypes.Contains(AttackableActor->GetRootComponent()->GetCollisionObjectType()))
	{
		return false;
	}

	IFQPlayerAttackableInterface* PlayerAttackableInterface = Cast<IFQPlayerAttackableInterface>(AttackableActor);
	if (!PlayerAttackableInterface)
	{
		return false;
	}

	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.f;
	Forward.Normalize();

	ApplyDamageToTarget(mMageDataAsset->mProjectileDamageAmount, AttackableActor);
	PlayerAttackableInterface->TakePushByPlayer(AttackableActor, Forward, mMageDataAsset->mProjectileStrength);

	return true;
}

void AFQMagePlayer::ProcessHitInterrupt()
{
	// R 공격 도중 피격 상태로 전환
	if (mbLaserCoolDown)
	{
		GetWorld()->GetTimerManager().ClearTimer(mLaserCoolTimer);
		GetWorld()->GetTimerManager().ClearTimer(mLaserDamageTimer);
		GetWorld()->GetTimerManager().ClearTimer(mLaserDurationTimer);

		mbLaserCoolDown = false;
		mLaserState = EMageLaserState::CoolDown;

		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]() { mLaserState = EMageLaserState::None; });
		GetWorld()->GetTimerManager().SetTimer(mLaserCoolTimer, TimerDel, mMageDataAsset->mLaserCoolTime, false);
	}
	else
	{
		mLaserState = EMageLaserState::None;
	}

	mStaff->DeactivateLaserEffect();
	mStaff->DeactivateHitEffect();
	mStaff->StopLaserAudio();

	mMoveState = EMoveState::CanMove;
}

float AFQMagePlayer::GetCircleMaxScale()
{
	if (!mMageDataAsset)
	{
		return 1.0f;
	}

	return mMageDataAsset->mMaxScale;
}

void AFQMagePlayer::ProcessProjectileAttack()
{
	if (!mProjectileClass)
	{
		return;
	}

	// 투사체 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	AFQMageProjectile* Projectile = GetWorld()->SpawnActor<AFQMageProjectile>(mProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if (Projectile)
	{
		// 투사체 초기화 및 발사
		Projectile->SetLifeSpan(mMageDataAsset->mProjectileDuration);
		Projectile->SetCount(mMageDataAsset->mProjectileCount);
		Projectile->LaunchProjectile(mLookAtDirection, mMageDataAsset->mProjectileSpeed);
		Projectile->PlayProjectileAudio();

		UE_LOG(LogTemp, Log, TEXT("[ProcessProjectileAttack] 생성 성공"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[ProcessProjectileAttack] 생성 실패"));
	}
}

void AFQMagePlayer::CheckProjectileAttackVolume()
{
	if (mMoveDirection.IsZero())
	{
		mLookAtDirection = GetActorForwardVector();
	}
	else
	{
		mLookAtDirection = mMoveDirection;
	}

	mLookAtRotation = mLookAtDirection.Rotation();

	float Distance = TNumericLimits<float>::Max();
	TArray<AActor*> OverlappedActors;
	mProjectileAttackVolume->GetOverlappingActors(OverlappedActors);
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

		if (RootComp->GetCollisionObjectType() != ECollisionChannel::ECC_GameTraceChannel3)
		{
			continue;
		}

		float NewDistance = FVector::Distance(GetActorLocation(), Actor->GetActorLocation());

		if (Distance <= NewDistance)
		{
			continue;
		}

		Distance = NewDistance;
		mLookAtDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		mLookAtDirection.Z = 0.0f;
		mLookAtRotation = mLookAtDirection.Rotation();
	}
}
