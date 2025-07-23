


#include "FQMonsterManager.h"
#include "Kismet/GameplayStatics.h"
#include "FQMonsterAIController.h"

// Sets default values
AFQMonsterManager::AFQMonsterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFQMonsterManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFQMonsterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFQMonsterManager::RegisterGroup(FName ID, AFQMonsterBase* Monster)
{
	if (!mMonsterGroup.Contains(ID))
	{
		mMonsterGroup.Add(ID, TArray<TObjectPtr<AFQMonsterBase>>());
	}
	mMonsterGroup[ID].Add(Monster);
}

void AFQMonsterManager::DeleteGroup(FName ID, AFQMonsterBase* Monster)
{
	mMonsterGroup[ID].Remove(Monster);
}

void AFQMonsterManager::SetTargetActor(FName ID, AActor* Actor)
{
	if (ID == TEXT("None"))
		ID = TEXT("Default");
	for (auto& Monster : mMonsterGroup[ID])
	{
		if(Monster)
		{
			if (Monster->mMonsterState == EMonsterState::Death)
				continue;

			AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(Monster->GetController());
			if (!IsValid(Monster->mTargetActor))
			{
				if (AIC)
				{
					AIC->SetTargetActor(Actor);
				}
			}
		}
	}
}

void AFQMonsterManager::ChangeTargetActor(FName ID, AActor* Actor)
{
	if (ID == TEXT("None"))
		ID = TEXT("Default");
	for (auto& Monster : mMonsterGroup[ID])
	{
		AFQMonsterAIController* AIC = Cast<AFQMonsterAIController>(Monster->GetController());
		if (AIC)
		{
			AIC->ChangeTargetActor(Actor);
		}
	}
}



