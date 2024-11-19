// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInfoWidget.h"

#include "Components/WidgetSwitcher.h"

void UNPCInfoWidget::SelectContent(int32 idx)
{
	WidgetSwitcher_->SetActiveWidgetIndex(idx);
}
