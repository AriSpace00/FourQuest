// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "FQGameModeUIInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFQGameModeUIInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FQGAMECORE_API IFQGameModeUIInputInterface
{
	GENERATED_BODY()

public:
	virtual void StartGame() PURE_VIRTUAL(IFQGameModeUIInputInterface::StartGame, );
	virtual void ExitGame() PURE_VIRTUAL(IFQGameModeUIInputInterface::ExitGame, );

	// UI Input Funtion
	virtual void MoveButton(const FInputActionValue& Value, int32 ControllerId) PURE_VIRTUAL(IFQGameModeUIInputInterface::MoveButton, );
	virtual void CancelInteraction(int32 ControllerId) PURE_VIRTUAL(IFQGameModeUIInputInterface::CancelInteraction, );
	virtual void SelectInteraction(int32 ControllerId) PURE_VIRTUAL(IFQGameModeUIInputInterface::SelectInteraction, );
	virtual void MenuInteraction() PURE_VIRTUAL(IFQGameModeUIInputInterface::MenuInteraction, );
};
