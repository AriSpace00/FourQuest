// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQUI/FQUserWidget.h"
#include "FQMonsterHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class FQUI_API UFQMonsterHpWidget : public UFQUserWidget
{
	GENERATED_BODY()
	
public:
	UFQMonsterHpWidget();

	UFUNCTION(BlueprintCallable, Category = "FQUpdateWidget")
	void UpdateHp(float HpValue);

private:
	// Parent Function
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float mHpPercent;
	float mHpDecrasePercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FQSign, Meta = (AllowPrivateAccess = "true"))
	float mHpDecraseSpeed;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mHp;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mHpDecrase;
};
