// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlotWidget.h"

#include "Components/TextBlock.h"

void UQuestSlotWidget::SetText_QuestName(FString str)
{
	Text_QuestName->SetText(FText::FromString(*str));
}
