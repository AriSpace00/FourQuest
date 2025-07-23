#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FQQuestStateBase.generated.h"

UCLASS()
class FQQUEST_API UFQQuestStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFQQuestStateBase();

	// 가상 함수
	virtual void EnterState() PURE_VIRTUAL(UFQQuestStateBase::StateEnter, );
	virtual void UpdateState(float DeltaTime) PURE_VIRTUAL(UFQQuestStateBase::StateUpdate, );
	virtual void ExitState() PURE_VIRTUAL(UFQQuestStateBase::StateExit, );

	// 겟셋 함수
	FORCEINLINE class AFQQuestBase* GetOwnerQuestObject() const { return mOwnerQuestObject; }
	FORCEINLINE void SetOwnerQuestObject(class AFQQuestBase* OwnerQuestObject) { mOwnerQuestObject = OwnerQuestObject; }

private:
	UPROPERTY()
	TObjectPtr<class AFQQuestBase> mOwnerQuestObject;
};
