// Fill out your copyright notice in the Description page of Project Settings.


#include "SavedSlotWidget.h"
#include "Components/TextBlock.h"

void USavedSlotWidget::SetText_Date(const FString& str)
{
	Text_Date->SetText(FText::FromString(str));
}

void USavedSlotWidget::SetText_Loc(const FString& str)
{
	Text_Loc->SetText(FText::FromString(str));
}
