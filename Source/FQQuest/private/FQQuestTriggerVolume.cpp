#include "FQQuestTriggerVolume.h"

#include "Components/BoxComponent.h"

#include "FQGameCore\Player\FQPlayerCharacterInterface.h"
#include "FQGameCore\Soul/FQSoulCharacterInterface.h"
#include "FQGameCore\Quest\FQQuestSystem.h"

AFQQuestTriggerVolume::AFQQuestTriggerVolume()
	: mPlayerNumber()
	, mQuestInteractionType(EQuestInteractionType::None)
{
	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(mTrigger);
	mTrigger->SetBoxExtent(FVector(100.f));
	mTrigger->SetCollisionProfileName(TEXT("Trigger"));
	mTrigger->SetGenerateOverlapEvents(true);

	PrimaryActorTick.bCanEverTick = true;
}

void AFQQuestTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (mTrigger)
	{
		mTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFQQuestTriggerVolume::OnTriggerBeginOverlap);
		mTrigger->OnComponentEndOverlap.AddDynamic(this, &AFQQuestTriggerVolume::OnTriggerEndOverlap);
	}
}

void AFQQuestTriggerVolume::Tick(float DeltaTime)
{
	if (mQuestInteractionType == EQuestInteractionType::Capture && mPlayerNumber > 0)
	{
		mDurationTime += DeltaTime;
		if (mDurationTime < 1.f)
		{
			return;
		}
		mDurationTime = 0.f;

		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Capture, 1);
		}
	}
}

void AFQQuestTriggerVolume::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IFQPlayerCharacterInterface* ArmourPlayer = Cast<IFQPlayerCharacterInterface>(OtherActor);
	if (ArmourPlayer)
	{
		mPlayerNumber++;

		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem && mQuestInteractionType == EQuestInteractionType::Teleport)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Teleport, 1);
			return;
		}
	}

	IFQSoulCharacterInterface* SoulPlayer = Cast<IFQSoulCharacterInterface>(OtherActor);
	if (SoulPlayer)
	{
		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem && mQuestInteractionType == EQuestInteractionType::Teleport)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Teleport, 1);
		}
	}
}

void AFQQuestTriggerVolume::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IFQPlayerCharacterInterface* CollisionPlayer = Cast<IFQPlayerCharacterInterface>(OtherActor);
	if (CollisionPlayer)
	{
		mPlayerNumber--;

		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem && mQuestInteractionType == EQuestInteractionType::Teleport)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Teleport, -1);
			return;
		}
	}

	IFQSoulCharacterInterface* SoulPlayer = Cast<IFQSoulCharacterInterface>(OtherActor);
	if (SoulPlayer)
	{
		UFQQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UFQQuestSystem>();
		if (QuestSystem && mQuestInteractionType == EQuestInteractionType::Teleport)
		{
			QuestSystem->mInteractionDelegate.Broadcast(EQuestInteractionType::Teleport, -1);
		}
	}
}
