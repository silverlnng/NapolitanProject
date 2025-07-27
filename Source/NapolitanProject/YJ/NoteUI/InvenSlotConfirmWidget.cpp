// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenSlotConfirmWidget.h"

#include "InvenSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/Item/ItemActor.h"

void UInvenSlotConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PC=GetOwningPlayer<ATestPlayerController>();
	TestCharacter =PC->GetPawn<ATestCharacter>();
	
	Btn_Take->OnClicked.AddDynamic(this,&UInvenSlotConfirmWidget::OnClickTake);
	Btn_No->OnClicked.AddDynamic(this,&UInvenSlotConfirmWidget::OnClickNo);
}

void UInvenSlotConfirmWidget::OnClickTake()
{
	if (TestCharacter)
	{
		if (TestCharacter->curItem)
		{
			AItemActor* prevItem=TestCharacter->curItem;
			prevItem->SetActorHiddenInGame(true); // 이전 아이템은 안보이게 
			if (MyItem)
			{
				TestCharacter->curItem=MyItem;
				MyItem->SetActorHiddenInGame(false);
			}
		}
		else
		{
			if (MyItem)
			{
				TestCharacter->curItem=MyItem;
				MyItem->SetActorHiddenInGame(false);
			}	
		}
		
	}
	/*if (CurInvenSlot)
	{
		CurInvenSlot->
	}*/

	//현재 플레이어의 아이템 조사 
}

void UInvenSlotConfirmWidget::OnClickNo()
{
	//SetVisibility(ESlateVisibility::Hidden);
	if (MyItem)
	{
		MyItem->SetActorHiddenInGame(true);
	}
	TestCharacter->curItem=nullptr;
}

void UInvenSlotConfirmWidget::Set_ImgThumnail(UTexture2D* Texture)
{
	Img_Thumnail->SetBrushFromTexture(Texture);
}

void UInvenSlotConfirmWidget::Set_TextItemInfo(const FString& str) const
{
	Text_ItemInfo->SetText(FText::FromString(*str));
}
