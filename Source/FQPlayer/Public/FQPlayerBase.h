// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "FQGameCore/Player/FQPlayerCharacterInterface.h"
#include "FQGameCore/Player/FQPlayerAttackableInterface.h"
#include "FQPlayer/Public/FQPlayerActionState.h"

#include "FQPlayerBase.generated.h"

UCLASS()
class FQPLAYER_API AFQPlayerBase : public ACharacter, public IFQPlayerCharacterInterface, public IFQPlayerAttackableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFQPlayerBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void ProcessNextSection() PURE_VIRTUAL(AFQPlayerBase::ProcessNextSection, );
	virtual void EnableAttackVolume() PURE_VIRTUAL(AFQPlayerBase::EnableAttackVolume, );
	virtual void DisableAttackVolume() PURE_VIRTUAL(AFQPlayerBase::DisableAttackVolume, );
	virtual void ProcessHitInterrupt() PURE_VIRTUAL(AFQPlayerBase::ProcessHitInterrupt, );

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// GameplayStatics
	void ApplyDamageToTarget(float DamageAmount, AActor* Target);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	
	// IFQPlayerAttackableInterface
	virtual void TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength) override;

	// Animation
	bool IsHit();

protected:
	virtual void BeginPlay() override;

	// Setting
	virtual void SetInputMappingContext();

	virtual bool CanMove() PURE_VIRTUAL(AFQPlayerBase::CanMove, return true;);
	virtual void ProcessInputMovement() PURE_VIRTUAL(AFQPlayerBase::ProcessInputMovement, );

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mMoveAction;

	float mDefaultSpeed;

	void Move(const FInputActionValue& Value);

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mHitAnim;

	void Hit();

	UFUNCTION()
	void OnHitAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	// Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> mSoulEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mKnightEffectSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mMageEffectSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mWarriorEffectSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> mArcherEffectSystem;

	// State 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EHitState mHitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMoveState mMoveState;

	// X 버튼을 길게 누르고 있는 상태인지 확인하는 플래그
	uint8 mbIsPressedX : 1;

	// A 버튼을 누르고 있는 상태인지 확인하는 플래그
	uint8 mbIsPressedA : 1;

	FVector mMoveDirection;

private:
	void SetMovement();
};
