// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQPlayerBase.h"
#include "FQMagePlayer.generated.h"

/**
 * 
 */
UCLASS()
class FQPLAYER_API AFQMagePlayer : public AFQPlayerBase
{
	GENERATED_BODY()

public:
	// Default
	AFQMagePlayer();
	virtual void Tick(float DeltaSeconds) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Animation
	virtual void ProcessNextSection() override;

	bool IsEnabledExplosionCircle();
	bool IsEnabledLaser();

	// Projectile Attack
	virtual void EnableAttackVolume() override;
	virtual void DisableAttackVolume() override;

	void CheckProjectileAttackVolume();
	void ProcessProjectileAttack();
	bool ApplyPush(class AActor* AttackableActor);

	virtual void ProcessHitInterrupt() override;

	float GetCircleMaxScale();

protected:
	// Default
	virtual void BeginPlay() override;

	// Input
	virtual void SetInputMappingContext() override;
	virtual bool CanMove() override;
	virtual void ProcessInputMovement() override;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class UFQMageDataAsset> mMageDataAsset;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> mMageMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mProjectileAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mExplosionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> mLaserAction;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mProjectileAttackAnim1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mProjectileAttackAnim2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mExplosionStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mExplosionEndAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mLaserStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> mLaserEndAnim;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMageProjectileAttackState mProjectileAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EComboState mProjectileAttackComboState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMageExplosionState mExplosionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	EMageLaserState mLaserState;

	// Projectile Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> mProjectileAttackVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFQMageProjectile> mProjectileClass;

	// Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFQMageCircle> mExplosionCircleClass;

	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFQMageStaff> mStaffClass;

	TObjectPtr<class AFQMageStaff> mStaff;

private:
	FVector mLookAtDirection;
	FRotator mLookAtRotation;
	FRotator mLaserRotation;

	// Animation
	UFUNCTION()
	void OnMageAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Projectile Attack
	FTimerHandle mProjectileAttackComboTimer;
	FTimerHandle mProjectileAttackCoolTimer;

	void StartProjectileAttack();
	void PressedProjectileAttack();
	void EndProjectileAttack();

	void ResetProjectileAttackCombo();
	void ResetProjectileAttackCoolDown();

	// Explosion
	UPROPERTY()
	TObjectPtr<AFQMageCircle> mExplosionCircle;

	FTimerHandle mExplosionCoolTimer;

	void StartExplosion();
	void PressedExplosion();
	void EndExplosion();
	void ProcessExplosion();

	// Laser
	uint8 mbLaserCoolDown : 1;

	FTimerHandle mLaserDamageTimer;
	FTimerHandle mLaserCoolTimer;
	FTimerHandle mLaserDurationTimer;

	UPROPERTY()
	TObjectPtr<AActor> mCurrentLaserTarget;

	void StartLaser(const FInputActionValue& Value);
	void PressedLaser(const FInputActionValue& Value);
	void EndLaser();
	void ApplyLaserDamage();
	void UpdateLaser();
};
