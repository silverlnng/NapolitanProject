// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"

void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	//Btn_InvenSlot->SetIsEnabled(false);
	
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

void UInvenSlotWidget::OnItemAcquired()
{
	// 아이템 얻는다면 버튼 , 이미지 활성화 되도록
	Img_Thumnail->SetVisibility(ESlateVisibility::Visible);
	Btn_InvenSlot->SetIsEnabled(true);
}
