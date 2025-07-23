// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQGameCore\Common.h"
#include "FQUI\FQUserWidget.h"
#include "FQArmourWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQArmourWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
    UFQArmourWidget();

protected:
    virtual void NativeConstruct() override;

protected:
    // 타입별 텍스처 (에디터에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    UTexture2D* mKnightArmourTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQArmour)
    UTexture2D* mMagicArmourTexture;
};
