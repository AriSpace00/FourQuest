// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQGameCore\Common.h"
#include "FQSoulComplateUI.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQSoulComplateUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQSoulComplateUI();

	void UpdateSoulAnimation(ESoulType InSoulType);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSoul, Meta = (AllowPrivateAccess = "true"))
	TMap<ESoulType, TObjectPtr<UMaterial>> mSoulMaterial;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> m3DSoulAnimation;
};
