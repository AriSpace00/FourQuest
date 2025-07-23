// Fill out your copyright notice in the Description page of Project Settings.


#include "FQSoulComplateUI.h"

#include "Components\Image.h"
#include "Materials\Material.h"

UFQSoulComplateUI::UFQSoulComplateUI()
{
}

void UFQSoulComplateUI::UpdateSoulAnimation(ESoulType InSoulType)
{
	if (mSoulMaterial.Find(InSoulType))
	{
		m3DSoulAnimation->SetBrushFromMaterial(mSoulMaterial[InSoulType]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UFQSoulComplateUI %d] mSoulMaterial Is Not Find SoulType!!"), __LINE__);
	}
}
