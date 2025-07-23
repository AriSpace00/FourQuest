// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQQuestPointWidget.generated.h"

UENUM()
enum class EQuestSignType
{
	Boss		UMETA(DisplayName = "Boss"),
	Holygrail	UMETA(DisplayName = "Holygrail"),
	Infect		UMETA(DisplayName = "Infect"),
	Portal		UMETA(DisplayName = "Portal"),
};

UCLASS()
class FQUI_API UFQQuestPointWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQQuestPointWidget();

	// 일반 함수
	void UpdateQuestPosition(FVector2D DirectionDistance);
	void UpdateQuestActive(bool bIsActive);

	// 겟셋 함수
	FORCEINLINE void SetQuestSignType(EQuestSignType QuestSignType) { mQuestSignType = QuestSignType; }
	FORCEINLINE void SetQuestSignActive(const bool bIsSignActive) { mbIsQuestSignActive = bIsSignActive; }
	FORCEINLINE bool GetQuestSignActive() { return mbIsQuestSignActive; }
	FORCEINLINE void SetQuestID(const int32 QuestID) { mQuestID = QuestID; }
	FORCEINLINE int32 GetQuestID() { return mQuestID; }

protected:
	// 부모 함수
	virtual void NativeConstruct() override;

private:

private:
	int32 mQuestID;
	uint8 mbIsQuestActive : 1;
	uint8 mbIsQuestSignActive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSign, Meta = (AllowPrivateAccess = "true"))
	EQuestSignType mQuestSignType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSign, Meta = (AllowPrivateAccess = "true"))
	TMap<EQuestSignType, TObjectPtr<UTexture2D>> mQuestActiveSignTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSign, Meta = (AllowPrivateAccess = "true"))
	TMap<EQuestSignType, TObjectPtr<UTexture2D>> mQuestInactiveSignTexture;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSign;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mDistanceText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> mSignBox;
};
