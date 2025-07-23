#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FQGameCore\Common.h"
#include "FQQuestTriggerVolume.generated.h"

UCLASS()
class FQQUEST_API AFQQuestTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AFQQuestTriggerVolume();

protected:
	// 부모 가상 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	// 충돌 박스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FQBox)
	TObjectPtr<class UBoxComponent> mTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQQuest)
	EQuestInteractionType mQuestInteractionType;
	
private:
	int8 mPlayerNumber;
	float mDurationTime;
};
