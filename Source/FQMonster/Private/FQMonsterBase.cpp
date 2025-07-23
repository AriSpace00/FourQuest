


#include "FQMonsterBase.h"
#include "FQMonsterManager.h"
#include "FQMonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AFQMonsterBase::AFQMonsterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mAttackMontage = nullptr;
	mTargetActor = nullptr;

	mAttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collider"));
	mAttackBox->SetupAttachment(RootComponent);
	mAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCollision"));

	GetMesh()->bReceivesDecals = false;

	mbCanPush = true;
}

// Called when the game starts or when spawned
void AFQMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	mSpawnedLocation = GetActorLocation();
	mbCanPush = true;
	
	if (mMonsterDataAsset)
	{
		mCurrentHP = mMonsterDataAsset->mMaxHP;
		mCurrentHPPercent= mCurrentHP/ mMonsterDataAsset->mMaxHP;
	}
	
	for (TActorIterator<AFQMonsterManager> It(GetWorld()); It; ++It)
	{
		Manager = *It;
		if(mGroupID==TEXT("None"))
			It->RegisterGroup(TEXT("Default"), this);
		else
			It->RegisterGroup(mGroupID, this);
		break;
	}
	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		if (UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &AFQMonsterBase::OnHitMontageEnded);
			UE_LOG(LogTemp, Warning, TEXT("Delegate 등록"))
		}
	}
}

// Called every frame
void AFQMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFQMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFQMonsterBase::Attack()
{
	if (mAttackMontage)
	{
		FName SectionName = GetRandomSectionName();
		PlayAnimMontage(mAttackMontage, 1.0f, SectionName);
	}
}

void AFQMonsterBase::ManagerSetTargetActor(AActor* Actor)
{
	if (Actor == nullptr)
	{
		mTargetActor = nullptr;
		Manager->SetTargetActor(mGroupID, Actor);
		return;
	}
	
	Manager->SetTargetActor(mGroupID, Actor);
}


FName AFQMonsterBase::GetRandomSectionName()
{
	const TArray<FName> Sections = { TEXT("Attack1"), TEXT("Attack2"), TEXT("Attack3"), TEXT("Attack4"), TEXT("Attack5") };
	int32 Index = FMath::RandRange(0, Sections.Num() - 1);
	return Sections[Index];
}

void AFQMonsterBase::ApplyDamageToTarget()
{
	TArray<AActor*> Overlapped;
	mAttackBox->GetOverlappingActors(Overlapped, AActor::StaticClass());

	for (AActor* Target : Overlapped)
	{
		if (Target && Target != this)
		{
			UGameplayStatics::ApplyDamage(Target, mMonsterDataAsset->mAttackPower, GetController(), this, UDamageType::StaticClass());
			if (mAttackHitSoundCue)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), mAttackHitSoundCue, mAttackBox->GetComponentLocation());
			}
		}
	}	
}

void AFQMonsterBase::SetCollisionEnabled(bool CollisionEnabled)
{
	if (CollisionEnabled)
		mAttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		mAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQMonsterBase::ChangeState(EMonsterState NewState)
{
	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		AIC->ChangeState(NewState);
	}
}

float AFQMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
	{
		AIC->ChangeState(EMonsterState::Hit);
	}

	return DamageAmount;
}

void AFQMonsterBase::DeleteActor(FName ID, AFQMonsterBase* Monster)
{
	if (ID == TEXT("None"))
		ID = FName("Default");
	Manager->DeleteGroup(ID, Monster);
}



void AFQMonsterBase::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == mHitMontage && !bInterrupted)
	{
		AFQMonsterAIController* AIC = GetController<AFQMonsterAIController>();
		if (AIC)
		{
			if(mMonsterState!=EMonsterState::Down&&mMonsterState!=EMonsterState::Death)
			{
				AIC->ChangeState(EMonsterState::Chase);
				UE_LOG(LogTemp, Warning, TEXT("Hit Animation 종료"))
			}
		}
	}
}

void AFQMonsterBase::TakePushByPlayer(AActor* Target, const FVector& Direction, float Strength)
{
	if (!mbCanPush)
	{
		return;
	}
	if (!Target)
	{
		return;
	}

	if (Target != this)
	{
		return;
	}

	if (mMonsterState != EMonsterState::Hit&&mMonsterState != EMonsterState::Death)
	{
		if (AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(GetController()))
		{
			AIC->StopMovement();
			AIC->ChangeState(EMonsterState::Hit);
		}
	}
	
	if (mMonsterState != EMonsterState::Death)
	{
		LaunchCharacter(Direction * Strength, true, true);
	}
}


