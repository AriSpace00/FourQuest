// Fill out your copyright notice in the Description page of Project Settings.


#include "FQWidgetComponent.h"
#include "FQUserWidget.h"

void UFQWidgetComponent::InitWidget()
{
	Super::InitWidget();
}

void UFQWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UFQUserWidget* FQUserWidget = Cast<UFQUserWidget>(GetWidget());
	if (FQUserWidget)
	{
		FQUserWidget->SetOwningActor(GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQWidgetComponent %d] UFQUserWidget Is Not Vaild!!"), __LINE__);
	}
}
