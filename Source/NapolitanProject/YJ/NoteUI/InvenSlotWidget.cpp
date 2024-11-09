// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotWidget.h"

#include "Components/Button.h"

void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_InvenSlot->OnHovered.AddDynamic(this,&UInvenSlotWidget::OnClickedInvenSlot);
	Btn_InvenSlot->OnClicked.AddDynamic(this,&UInvenSlotWidget::OnClickedInvenSlot);
}

void UInvenSlotWidget::OnClickedInvenSlot()
{
	// 자신의 ItemIndex 으로 
	// 해당 메쉬를 카메라앞에 스폰되도록 하기
	//GI->DT_itemData->FindRow<FItemData>
}

void UInvenSlotWidget::OnHoverInvenSlot()
{
}
