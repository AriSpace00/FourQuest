// Fill out your copyright notice in the Description page of Project Settings.


#include "FQArmourBase.h"

#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Blueprint\UserWidget.h"

#include "FQGameCore\Soul\FQSoulCharacterInterface.h"
#include "FQUI\FQWidgetComponent.h"
#include "FQUI\Armour\FQArmourWidget.h"

// Sets default values
AFQArmourBase::AFQArmourBase()
	:mArmourHp(100)
{
	PrimaryActorTick.bCanEverTick = true; // Tick 함수가 실행되기 위한 bool형 타입 변수
	mArmourType = EArmourType::Knight;

	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mArmourWidget = CreateDefaultSubobject<UFQWidgetComponent>(TEXT("ArmourWidget"));

	RootComponent = mTrigger;
	mMesh->SetupAttachment(mTrigger);
	mArmourWidget->SetupAttachment(mTrigger);

	//mTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	mTrigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	mTrigger->SetCollisionProfileName(TEXT("ArmourCollision"));

	// Mesh
	mMesh->SetRelativeLocation(FVector(0.0f, -3.5f, 0.0f));
	mMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
	mMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Widget Component
	mArmourWidget->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	static ConstructorHelpers::FClassFinder<UFQArmourWidget> ArmourWidgetRef(TEXT("/Game/Blueprints/Armour/WBP_ArmourType.WBP_ArmourType_C"));
	if (ArmourWidgetRef.Class)
	{
		mArmourWidget->SetWidgetClass(ArmourWidgetRef.Class);
		mArmourWidget->SetWidgetSpace(EWidgetSpace::Screen);
		mArmourWidget->SetDrawSize(FVector2D(10.f, 10.f));
		mArmourWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mArmourWidget->SetVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQArmourBase %d] Failed UFQArmourWidget Class Reference Find!!"), __LINE__);
	}
}

void AFQArmourBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mMesh->AddRelativeRotation(FRotator(0.f, 1.f, 0.f));
}

// Called when the game starts or when spawned
void AFQArmourBase::BeginPlay()
{
	Super::BeginPlay();
}



#pragma region Interface
void AFQArmourBase::PickArmour()
{
	Destroy();
}

void AFQArmourBase::SetNearestArmour(bool IsTrue)
{
	if (mArmourWidget)
	{
		mArmourWidget->SetVisibility(IsTrue);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AFQArmourBase %d] Is Not Valid mArmourWidget"), __LINE__);
	}
}

EArmourType AFQArmourBase::GetArmourType() const
{
	return mArmourType;
}

FTransform AFQArmourBase::GetActorTransform() const
{
	return GetTransform();
}
#pragma endregion