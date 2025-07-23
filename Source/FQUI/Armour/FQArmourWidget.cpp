// Fill out your copyright notice in the Description page of Project Settings.


#include "FQArmourWidget.h"

#include "Components\Image.h"
#include "Blueprint/UserWidget.h"

#include "FQGameCore\Armour\FQArmourInterface.h"

UFQArmourWidget::UFQArmourWidget()
{
}

void UFQArmourWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (mKnightArmourTexture == nullptr || mMagicArmourTexture == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQArmourWidget %d] KnightArmour Or MagicArmour 텍스쳐가 없습니다!!"), __LINE__);
        return;
    }

    IFQArmourInterface* OwnerArmour = Cast<IFQArmourInterface>(mOwningActor);
    if (!OwnerArmour)
    {
        return;
    }

    EArmourType OwnerArmourType = OwnerArmour->GetArmourType();
    UTexture2D* SelectedTexture = nullptr;
    switch (OwnerArmourType)
    {
    case EArmourType::Knight:
        SelectedTexture = mKnightArmourTexture;
        break;
    case EArmourType::Magic:
        SelectedTexture = mMagicArmourTexture;
        break;
    default:
        UE_LOG(LogTemp, Error, TEXT("[UFQArmourWidget %d] Enum SoulType이 존재하지 않습니다!!"), __LINE__);
        break;
    }

    UImage* ArmourTypeImage = Cast<UImage>(GetWidgetFromName(TEXT("ArmourType")));
    if (ArmourTypeImage)
    {
        ArmourTypeImage->SetBrushFromTexture(SelectedTexture);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQArmourWidget %d] ArmourTypeImage Is Not Find 'ArmourType' Widget Component!!"), __LINE__);
    }
}
