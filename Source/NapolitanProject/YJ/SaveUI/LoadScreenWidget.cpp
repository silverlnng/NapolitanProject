// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadScreenWidget.h"

#include "SaveConfirmWidget.h"
#include "SaveSlotSwitcherWidget.h"
#include "VacantSaveWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

void ULoadScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SaveSlotSwitcherList.Add(0, SaveSlotSwitcherWidget_0);
	SaveSlotSwitcherList.Add(1, SaveSlotSwitcherWidget_1);
	SaveSlotSwitcherList.Add(2, SaveSlotSwitcherWidget_2);
	int index = 0;
	for (auto& i : SaveSlotSwitcherList)
	{
		i.Value->SlotNumber=index;
		i.Value->WBP_VacantSlot->SlotNumber=index;
		index++;
	}

		SaveSlotSwitcherWidget_0->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnSaveQuestionWidgetCreate_0);
		//SaveSlotSwitcherWidget_1->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic()
		//SaveSlotSwitcherWidget_2->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic()
	
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_0()
{
	
	// 위젯생성하고
	SaveConfirmWidget =CreateWidget<USaveConfirmWidget>(this,SaveQuestWidgetFactory);

	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveConfirmWidget->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_0;
	
	CanvasPanel->AddChild(SaveConfirmWidget);
	
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_1()
{
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_2()
{
}
