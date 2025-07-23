

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQMonsterBase.h"
#include "FQMonsterManager.generated.h"

UCLASS()
class FQMONSTER_API AFQMonsterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFQMonsterManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RegisterGroup(FName ID,AFQMonsterBase* Monster);

	void DeleteGroup(FName ID, AFQMonsterBase* Monster);

	void SetTargetActor(FName ID, AActor* Actor);

	void ChangeTargetActor(FName ID, AActor* Actor);
private:
	TMap < FName, TArray<TObjectPtr<AFQMonsterBase>>> mMonsterGroup;

	
};
