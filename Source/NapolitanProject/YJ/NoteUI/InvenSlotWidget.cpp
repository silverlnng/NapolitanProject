// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "NapolitanProject/Interact/ItemActor.h"

void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	//Btn_InvenSlot->SetIsEnabled(false);

	// 인벤슬롯 초기화
	Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	Btn_InvenSlot->SetIsEnabled(false);
	
	Btn_InvenSlot->OnHovered.AddDynamic(this,&UInvenSlotWidget::OnClickedInvenSlot);
	Btn_InvenSlot->OnClicked.AddDynamic(this,&UInvenSlotWidget::OnClickedInvenSlot);
}

void UInvenSlotWidget::OnClickedInvenSlot()
{
	// 자신의 ItemIndex 으로 해당하는 ItemAcotr의 함수실행시키기  
	// 해당 메쉬를 카메라앞에 스폰되도록 하기
		// 중복되면 안됨. 먼저 앞에들고있는걸 해제하고. 
	//GI->DT_itemData->FindRow<FItemData>

	if (MyItem)
	{
		MyItem->Use();
	}
}

void UInvenSlotWidget::OnHoverInvenSlot()
{
	// 사용 을 묻는 ui 나오도록 하기 
}

void UInvenSlotWidget::OnItemAcquired()
{
	// 아이템 얻는다면 버튼 , 이미지 활성화 되도록
	Img_Thumnail->SetVisibility(ESlateVisibility::Visible);
	Btn_InvenSlot->SetIsEnabled(true);
}
