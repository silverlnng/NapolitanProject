// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotWidget.h"

#include "InvenSlotConfirmWidget.h"
#include "InventoryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInvenSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	//Btn_InvenSlot->SetIsEnabled(false);

	// 인벤슬롯 초기화
	Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	Btn_InvenSlot->SetIsEnabled(false);
	
	// 호버할때 소리 넣기 
	Btn_InvenSlot->OnHovered.AddDynamic(this,&UInvenSlotWidget::OnHoverPlaySound);
	
	Btn_InvenSlot->OnClicked.AddDynamic(this,&UInvenSlotWidget::OnClickedInvenSlot);
}

void UInvenSlotWidget::OnHoverPlaySound()
{
	if (HoverSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, HoverSoundWave);
	}
}

void UInvenSlotWidget::OnClickedInvenSlot()
{
	// 자신의 ItemIndex 으로 해당하는 ItemAcotr의 함수실행시키기  
	// 해당 메쉬를 카메라앞에 스폰되도록 하기
		// 중복되면 안됨. 먼저 앞에들고있는걸 해제하고. 
	//GI->DT_itemData->FindRow<FItemData>

	if (InventoryUI&&InventoryUI->WBP_InvenSlot_Confirm)
	{
		InventoryUI->WBP_InvenSlot_Confirm->CurInvenSlot=this;
		InventoryUI->WBP_InvenSlot_Confirm->Set_ImgThumnail(Mytexture2D);
		InventoryUI->WBP_InvenSlot_Confirm->SetVisibility(ESlateVisibility::Visible);
		InventoryUI->WBP_InvenSlot_Confirm->Set_TextItemInfo(ItemInfo);
		InventoryUI->WBP_InvenSlot_Confirm->MyItem=MyItem;
	}
}

void UInvenSlotWidget::OnItemAcquired()
{
	// 아이템 얻는다면 버튼 , 이미지 활성화 되도록
	Img_Thumnail->SetVisibility(ESlateVisibility::Visible);
	Btn_InvenSlot->SetIsEnabled(true);
}

void UInvenSlotWidget::Set_TextNum(const FString& str) const
{
	Text_Num->SetText(FText::FromString(*str));
}

void UInvenSlotWidget::SetIsEnabledBtn()
{
	// 인벤슬롯 초기화
	Img_Thumnail->SetVisibility(ESlateVisibility::Hidden);
	Btn_InvenSlot->SetIsEnabled(false);
}
