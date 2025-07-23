// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FQWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;

public:
	virtual void BeginPlay() override;
};
