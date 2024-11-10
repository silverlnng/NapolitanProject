// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InvenSlotWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//ItemActor에 있는  ItemMap에 있는 정보로 slot을 초반에 생성시키고 싶다
	for (int i=0;i<8;i++)
	{
		
		UInvenSlotWidget* newSlot=CreateWidget<UInvenSlotWidget>(this,InvenSlotWidgetFactory);
		int row = i/2;
		int colume=i%2;
		GridPanel_Slot->AddChildToUniformGrid(newSlot,row,colume);
		newSlot->ItemIndex=i+1;
		InvenSlots.Add(i,newSlot);
	}
}
