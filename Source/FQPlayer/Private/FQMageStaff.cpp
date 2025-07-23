// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageStaff.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AFQMageStaff::AFQMageStaff()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = mStaticMesh;
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mLaserEffectLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LaserEffectLocation"));
	mLaserEffectLocation->SetupAttachment(RootComponent);

	mLaserEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserEffect"));
	mLaserEffect->SetAutoActivate(false);
	mLaserEffect->SetupAttachment(RootComponent);

	mHitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitEffect"));
	mHitEffect->SetAutoActivate(false);
	mHitEffect->SetupAttachment(RootComponent);

	mLaserAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("LaserAudio"));
	mLaserAudio->SetupAttachment(RootComponent);
	mLaserAudio->SetAutoActivate(false);

	mHitAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudio"));
	mHitAudio->SetupAttachment(RootComponent);
	mHitAudio->SetAutoActivate(false);
}


// Called when the game starts or when spawned
void AFQMageStaff::BeginPlay()
{
	Super::BeginPlay();

	mLaserAudio->RegisterComponent();
	mHitAudio->RegisterComponent();
}

// Called every frame
void AFQMageStaff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFQMageStaff::ActivateLaserEffect(const FVector& End)
{
	if (!mLaserEffect)
	{
		return;
	}

	if (!mLaserEffect->IsActive())
	{
		mLaserEffect->Activate();
	}

	mLaserEffect->SetVectorParameter(FName(TEXT("BeamStart")), mLaserEffectLocation->GetComponentLocation());
	mLaserEffect->SetVectorParameter(FName(TEXT("LazerEnd")), End);
}

void AFQMageStaff::DeactivateLaserEffect()
{
	if (!mLaserEffect)
	{
		return;
	}

	if (!mLaserEffect->IsActive())
	{
		return;
	}

	mLaserEffect->Deactivate();

	if (!mLaserAudio)
	{
		return;
	}
}

void AFQMageStaff::ActivateHitEffect(const FVector& End)
{	
	if (!mHitEffect)
	{
		return;
	}

	if (!mHitEffect->IsActive())
	{
		mHitEffect->Activate();
	}
	
	mHitEffect->SetWorldLocation(End);

	if (!mHitAudio->IsPlaying())
	{
		mHitAudio->Play();
	}
}

void AFQMageStaff::DeactivateHitEffect()
{
	if (!mHitEffect)
	{
		return;
	}

	if (!mHitEffect->IsActive())
	{
		return;
	}

	mHitEffect->Deactivate();
}

void AFQMageStaff::PlayLaserAudio()
{
	if (!mLaserAudio)
	{
		return;
	}

	if (!mLaserAudio->IsPlaying())
	{
		mLaserAudio->Play();
	}	
}

void AFQMageStaff::StopLaserAudio()
{
	if (!mLaserAudio)
	{
		return;
	}

	mLaserAudio->Stop();
}

const FVector AFQMageStaff::GetLaserEffectLocation()
{
	return mLaserEffectLocation->GetComponentLocation();
}

