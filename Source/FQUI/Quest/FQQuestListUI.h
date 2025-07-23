// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQQuestListUI.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQQuestListUI : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQQuestListUI();

	void AddQuestListWidget(UFQUserWidget* QuestWidget);
	void RemoveQuestWidget(UFQUserWidget* QuestWidget);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> mQuestListBox;
};
