// Fill out your copyright notice in the Description page of Project Settings.


#include "FQRangeMonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
AFQRangeMonsterProjectile::AFQRangeMonsterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    mCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    mCollisionComp->InitSphereRadius(5.0f);
    mCollisionComp->SetCollisionProfileName("Projectile");
    mCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFQRangeMonsterProjectile::OnHit);
    RootComponent = mCollisionComp;

    mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    mProjectileMovement->InitialSpeed = 1500.f;
    mProjectileMovement->MaxSpeed = 1500.f;
    mProjectileMovement->bRotationFollowsVelocity = true;
    mProjectileMovement->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AFQRangeMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFQRangeMonsterProjectile::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this&&OtherActor!=GetOwner())
    {
        UGameplayStatics::ApplyDamage(OtherActor, mDamage, GetInstigatorController(), this, nullptr);
        if(mAttackHitSoundCue)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), mAttackHitSoundCue, GetActorLocation());
        }
        if (mProjectileHitEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                mProjectileHitEffect,
                GetActorLocation(),
                GetActorRotation()
            );
        }
        Destroy();
    }

}

void AFQRangeMonsterProjectile::SetProjectileSpeed(float Speed)
{
    mProjectileMovement->InitialSpeed = Speed;
    mProjectileMovement->MaxSpeed = Speed;

    mProjectileMovement->Velocity = mProjectileMovement->Velocity.GetSafeNormal() * Speed;
}



