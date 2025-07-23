// Fill out your copyright notice in the Description page of Project Settings.


#include "FQPlayer/Public/FQMageCircle.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"

#include "FQPlayer/Public/FQMagePlayer.h"

// Sets default values
AFQMageCircle::AFQMageCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mInitBoxExtent = FVector(128.0f, 128.0f, 32.0f);
	mInitDecalSize = FVector(128.0f, 128.0f, 128.0f);

	mVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	mVolume->SetBoxExtent(mInitBoxExtent);
	RootComponent = mVolume;

	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	mEffect->SetupAttachment(RootComponent);
	mEffect->SetAutoActivate(false);

	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	mDecal->DecalSize = mInitDecalSize;
	mDecal->SetupAttachment(RootComponent);

	mCircleAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CircleAudio"));
	mCircleAudio->SetupAttachment(RootComponent);
	mCircleAudio->SetAutoActivate(false);

	mExplosionAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionAudio"));
	mExplosionAudio->SetupAttachment(RootComponent);
	mExplosionAudio->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AFQMageCircle::BeginPlay()
{
	Super::BeginPlay();

	AFQMagePlayer* Player = Cast<AFQMagePlayer>(GetOwner());
	if (!Player)
	{
		return;
	}

	SetScale(Player->GetCircleMaxScale());
	UpdateComponentTransforms();

	mVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mEffect->OnSystemFinished.AddDynamic(this, &AFQMageCircle::OnEffectFinished);

	mCircleAudio->RegisterComponent();
	mExplosionAudio->RegisterComponent();
}

// Called every frame
void AFQMageCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFQMageCircle::ActivateEffect()
{
	if (!mEffect)
	{
		return;
	}

	mEffect->Activate();
	mDecal->SetVisibility(false);
}

void AFQMageCircle::SetScale(float ScaleFactor)
{
	FVector VolumeExtent = mInitBoxExtent;
	FVector NewVolumeExtent = FVector(VolumeExtent.X * ScaleFactor, VolumeExtent.Y * ScaleFactor, VolumeExtent.Z);
	mVolume->SetBoxExtent(NewVolumeExtent);

	FVector DecalSize = mInitDecalSize;
	FVector NewDecalSize = FVector(DecalSize.X, DecalSize.Y * ScaleFactor, DecalSize.Z * ScaleFactor);
	mDecal->DecalSize = NewDecalSize;
}

void AFQMageCircle::OnEffectFinished(UNiagaraComponent* PSystem)
{
	if (PSystem != mEffect)
	{
		return;
	}

	Destroy();
}

void AFQMageCircle::PlayCircleAudio()
{
	if (!mCircleAudio)
	{
		return;
	}

	if (!mCircleAudio->IsPlaying())
	{
		mCircleAudio->Play();
	}
}

void AFQMageCircle::StopCircleAudio()
{
	if (!mCircleAudio)
	{
		return;
	}

	mCircleAudio->Stop();
}

void AFQMageCircle::PlayExplosionAudio()
{
	if (!mExplosionAudio)
	{
		return;
	}

	if (!mExplosionAudio->IsPlaying())
	{
		mExplosionAudio->Play();
	}
}



