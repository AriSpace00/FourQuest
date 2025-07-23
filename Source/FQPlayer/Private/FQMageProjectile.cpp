// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "FQPlayer/Public/FQMagePlayer.h"

// Sets default values
AFQMageProjectile::AFQMageProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	RootComponent = mVolume;

	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	mMovement->UpdatedComponent = mVolume;
	mMovement->bRotationFollowsVelocity = false;
	mMovement->bShouldBounce = false;
	mMovement->ProjectileGravityScale = 0.0f;

	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	mEffect->SetupAttachment(RootComponent);
	mEffect->SetAutoActivate(true);

	mProjectileAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ProjectileSound"));
	mProjectileAudio->SetupAttachment(RootComponent);
	mProjectileAudio->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AFQMageProjectile::BeginPlay()
{
	Super::BeginPlay();

	mVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mVolume->OnComponentBeginOverlap.AddDynamic(this, &AFQMageProjectile::OnVolumeBeginOverlap);

	mProjectileAudio->RegisterComponent();
}

void AFQMageProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (mDestroyAudio)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, mDestroyAudio, GetActorLocation());
	}
}

// Called every frame
void AFQMageProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFQMageProjectile::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Owner)
	{
		return;
	}

	if (OtherActor == Owner)
	{
		return;
	}

	AFQMagePlayer* Player = Cast<AFQMagePlayer>(Owner);

	if (!Player)
	{
		return;
	}
	if (mHitActors.Contains(OtherActor))
	{
		return;
	}
	if (mCount <= 0)
	{
		mVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (mDestroyAudio)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, mDestroyAudio, GetActorLocation());
		}
		
		mProjectileAudio->Stop();

		Destroy();

		return;
	}

	if (Player->ApplyPush(OtherActor))
	{
		mHitActors.Add(OtherActor);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mHitEffectSystem, GetActorLocation(), GetActorRotation(), FVector(1.0f), true, true);

		mCount--;
	}
}

void AFQMageProjectile::LaunchProjectile(const FVector& Direction, float Speed)
{
	mMovement->Velocity = Direction * Speed;
}

void AFQMageProjectile::SetCount(int32 Count)
{
	mCount = Count;
}

void AFQMageProjectile::PlayProjectileAudio()
{
	if (!mProjectileAudio)
	{
		return;
	}

	if (!mProjectileAudio->IsPlaying())
	{
		mProjectileAudio->Play();
	}
}

