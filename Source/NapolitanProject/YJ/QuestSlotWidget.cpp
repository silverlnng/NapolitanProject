// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlotWidget.h"

#include "Components/TextBlock.h"

void UQuestSlotWidget::SetText_QuestName(const FString& str) const
{
	Text_QuestName->SetText(FText::FromString(*str));
}

int32 UQuestSlotWidget::GetQuestNum()
{
	return QuestNum;
}

void UQuestSlotWidget::SetQuestNum(int32 val)
{
	QuestNum=val;
}
