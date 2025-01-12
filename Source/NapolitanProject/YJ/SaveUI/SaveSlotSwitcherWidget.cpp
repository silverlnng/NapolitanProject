// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlotSwitcherWidget.h"

#include "Components/WidgetSwitcher.h"

void USaveSlotSwitcherWidget::SetActiveIndex(int index)
{
	SlotSwitcher->SetActiveWidgetIndex(index);
}
