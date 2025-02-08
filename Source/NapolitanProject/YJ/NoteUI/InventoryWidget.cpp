// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InvenSlotConfirmWidget.h"
#include "InvenSlotWidget.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI =GetGameInstance<UMyTestGameInstance>();

	WBP_InvenSlot_Confirm->SetVisibility(ESlateVisibility::Hidden);
	
	//ItemActor에 있는  ItemMap에 있는 정보로 slot을 초반에 생성시키고 싶다
	for (int i=0;i<8;i++) //최대 8칸.
	{
		
		UInvenSlotWidget* newSlot=CreateWidget<UInvenSlotWidget>(this,InvenSlotWidgetFactory);
		int row = i/2;
		int colume=i%2;
		GridPanel_Slot->AddChildToUniformGrid(newSlot,row,colume);
		
		newSlot->ItemIndex=i+1;    // 개발편의를 위해 ItemIndex 는 1 부터 시작.(데이터테이블도 1부터시작 )
		newSlot->InventoryUI=this;
		
		InvenSlots.Add(i,newSlot);
	}
	
	// InvenSlot 들을 생성시키고 이미지들 할당
	
	if (GI)
	{
		for (int i = 0; i < GI->itemDataRowNames.Num(); i++)
		{
			// DT_itemData 에서 행 찾아서 
			FItemData* ItemData = GI->DT_itemData->FindRow<FItemData>(GI->itemDataRowNames[i] , TEXT(""));
			if (ItemData)
			{
				// 인벤토리 슬롯에 썸네일 이미지 할당
				if (InvenSlots.Contains(i))
				{
					InvenSlots[i]->Img_Thumnail->SetBrushFromTexture(ItemData->thumnail);
					InvenSlots[i]->Mytexture2D=ItemData->thumnail;
					InvenSlots[i]->ItemInfo=ItemData->ItemInfo;
				}
			}
		}
	}
	
}
