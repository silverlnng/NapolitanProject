// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"

#include "TestGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/SouvenirWidget.h"
#include "Components/Image.h"

void ATestPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlayerHUD=GetHUD<APlayerHUD>();
	GI = GetGameInstance<UTestGameInstance>();
}

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerHUD=GetHUD<APlayerHUD>();
	GI = GetGameInstance<UTestGameInstance>();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,[this]()
	{
		if (!PlayerHUD)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD is null"));
			return;
		}
		if (!PlayerHUD->NoteUI)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD->NoteUI is null"));
			return;
		}
		if (!PlayerHUD->NoteUI->WBP_Inventory)
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerHUD->NoteUI->WBP_Inventory is null"));
			return;
		}
		
		auto InvenSlotMap = PlayerHUD->NoteUI->WBP_Inventory->InvenSlots;
		//먼저 모든 행단위로 가져오기  
		
		for (int i = 0; i < GI->itemDataRowNames.Num(); i++)
		{
			FItemData* ItemData = GI->DT_itemData->FindRow<FItemData>(GI->itemDataRowNames[i] , TEXT(""));
			if (ItemData)
			{
				// 인벤토리 슬롯에 썸네일 이미지 할당
				InvenSlotMap[i]->Img_Thumnail->SetBrushFromTexture(ItemData->thumnail);
			}
		}
		
		SouvenirUI = PlayerHUD->NoteUI->WBP_Souvenir;
		SouvenirUI->SouvenirBtn_BackDele.AddDynamic(this , &ATestPlayerController::SetSouvenirUICurNumber);
		//초기값
		SetSouvenirUICurNumber(0);
		
	},1.0f,false);
	
}

void ATestPlayerController::SetSouvenirUICurNumber(int curNum)
{
	//GI 의 SouvenirMap에서 앍어오기
	 // curNum 을 fname으로 바꾸기

	if (0==curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (0!=curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Visible);}
	if (SouvenirUI->MaxPage==curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Hidden);}
	if (SouvenirUI->MaxPage!=curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Visible);}
	
	FName curNum_ = FName(FString::FromInt(curNum));
	FSouvenirData* SouvenirData = GI->DT_SouvenirData->FindRow<FSouvenirData>(curNum_,TEXT(""));
	if (!SouvenirData){return;} // 방어코드

	// SouvenirData->IsHad 값에 따라서 
	if(!SouvenirData->IsHad)
	{
		SouvenirUI->Img_Souvenir_left->SetColorAndOpacity(FLinearColor(0.01,0.01,0.01,1));
	}
	else
	{
		SouvenirUI->Img_Souvenir_left->SetColorAndOpacity(FLinearColor(1,1,1,1));
	}
	
	SouvenirUI->Img_Souvenir_left->SetBrushFromTexture(SouvenirData->thumnail);
	SouvenirUI->Text_Souvenir_left->SetText( FText::FromString(SouvenirData->SouvenirInfo));

	FName nextNum_ = FName(FString::FromInt(curNum+1));
	FSouvenirData* SouvenirDataNext = GI->DT_SouvenirData->FindRow<FSouvenirData>(nextNum_,TEXT(""));
	if (!SouvenirDataNext){return;} // 방어코드

	if(!SouvenirDataNext->IsHad)
	{
		SouvenirUI->Img_Souvenir_right->SetColorAndOpacity(FLinearColor(0.01,0.01,0.01,1));
	}
	else
	{
		SouvenirUI->Img_Souvenir_right->SetColorAndOpacity(FLinearColor(1,1,1,1));
	}
	
	SouvenirUI->Img_Souvenir_right->SetBrushFromTexture(SouvenirDataNext->thumnail);
	SouvenirUI->Text_Souvenir_right->SetText( FText::FromString(SouvenirDataNext->SouvenirInfo));
	
	
}
