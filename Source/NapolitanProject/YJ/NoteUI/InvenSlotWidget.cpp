// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotWidget.h"

#include "Components/Button.h"

void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_InvenSlot->OnHovered.AddDynamic(this,UInvenSlotWidget::OnClickedInvenSlot);
	Btn_InvenSlot->OnClicked.AddDynamic(this,UInvenSlotWidget::OnClickedInvenSlot);
}

void UInvenSlotWidget::OnClickedInvenSlot()
{
}

void UInvenSlotWidget::OnHoverInvenSlot()
{
}
