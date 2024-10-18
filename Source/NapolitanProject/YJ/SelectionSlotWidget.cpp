// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionSlotWidget.h"

#include "Components/TextBlock.h"

void USelectionSlotWidget::OnClickedSelection()
{
	// 버튼에 할당된 값과 지금 대화하는 npc의 값을 비교하기

	// 지금 대화하는 npc는 주인공 캐릭터를 통해 알수있

	SelectionSlotBtnDele.Broadcast(result);
	
}

void USelectionSlotWidget::SetText(FString str)
{
	Text_Selection->SetText(FText::FromString(str));
}
