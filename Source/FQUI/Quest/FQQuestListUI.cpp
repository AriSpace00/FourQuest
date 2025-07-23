#include "FQQuestListUI.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

UFQQuestListUI::UFQQuestListUI()
{
}

void UFQQuestListUI::AddQuestListWidget(UFQUserWidget* QuestWidget)
{
    if (!QuestWidget || !mQuestListBox)
    {
        UE_LOG(LogTemp, Error, TEXT("[UFQQuestListUI %d] QuestWidget Or mQuestListBox가 유효하지 않습니다!!"), __LINE__);
        return;
    }

    // VerticalBox에 위젯 추가
    UVerticalBoxSlot* NewSlot = mQuestListBox->AddChildToVerticalBox(QuestWidget);
    if (NewSlot)
    {
        // 슬롯 속성 설정
        NewSlot->SetPadding(FMargin(0.f, 10.f, 0.f, 10.f));
        NewSlot->SetHorizontalAlignment(HAlign_Left);
        NewSlot->SetVerticalAlignment(VAlign_Top);
    }
}

void UFQQuestListUI::RemoveQuestWidget(UFQUserWidget* QuestWidget)
{
    if (mQuestListBox && QuestWidget)
    {
        mQuestListBox->RemoveChild(QuestWidget);
    }
}
