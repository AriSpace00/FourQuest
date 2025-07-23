

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQGameCore/Common.h"
#include "FQMonsterDataAsset.h"
#include "Sound/SoundCue.h"
#include "FQGameCore/Player/FQPlayerAttackableInterface.h"
#include "FQGameCore/Quest/FQQuestSystem.h"
#include "FQMonsterBase.generated.h"

class AFQMonsterManager;
class UBoxComponent;

UENUM()
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Hit UMETA(DisplayName = "Hit"),
	Down UMETA(DisplayName = "Down"),
	Rush UMETA(DisplayName = "Rush"),
	Growl UMETA(DisplayName = "Growl"),
	Death UMETA(DisplayName = "Death")
};

UCLASS()
class FQMONSTER_API AFQMonsterBase : public ACharacter, public IFQPlayerAttackableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFQMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack();

	void ManagerSetTargetActor(AActor* Actor);
	
	FName GetRandomSectionName();

	void ApplyDamageToTarget();

	void SetCollisionEnabled(bool CollisionEnabled);

	void ChangeState(EMonsterState NewState);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void DeleteActor(FName ID, AFQMonsterBase* Monster);

	UFUNCTION()
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Stat")
	float mCurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float mCurrentHPPercent;

	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	float mAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	UFQMonsterDataAsset* mMonsterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterGroup")
	FName mGroupID=TEXT("None");

	FVector mSpawnedLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	EMonsterState mMonsterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
	UAnimMontage* mAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationMontage")
	UAnimMontage* mHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	AActor* mTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UBoxComponent* mAttackBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	EQuestMonsterType mMonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	USoundCue* mAttackHitSoundCue;

protected:
	AFQMonsterManager* Manager;

public:
	virtual void TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength) override;

	int8 mbCanPush : 1;
};
