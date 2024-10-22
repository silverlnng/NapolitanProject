// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"

#include "NPCCharacter.h"
#include "TestGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/SouvenirWidget.h"
#include "Components/Image.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

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

	////////// 버튼 보이게,안보이게 처리
	if (0==curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (0!=curNum){SouvenirUI->Btn_Souvenir_Back->SetVisibility(ESlateVisibility::Visible);}
	if (SouvenirUI->MaxPage==curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Hidden);}
	if (SouvenirUI->MaxPage!=curNum){SouvenirUI->Btn_Souvenir_front->SetVisibility(ESlateVisibility::Visible);}
	//////////
	
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

void ATestPlayerController::SetNPCDialougueVisible(bool value)
{
	if (value)
	{
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATestPlayerController::SetNPCDialougueText()
{
	int32 npcID =curNPC->NPC_ID;
	int32 npcState =curNPC->State;

	// 이걸로 해당 대사의 처음 과 끝을 가져오기

	int32 count=0;
	int32 FindKey =(npcID*1000)+(npcState*100); // 시작하는 키값
	TArray<FString> str;
	
	
}

void ATestPlayerController::SetCurNPC(ANPCCharacter* curNPC_)
{
	curNPC = curNPC_;
}

void ATestPlayerController::CallCurNPCResultEvent(int32 value)
{
	UE_LOG(LogTemp,Warning,TEXT("%d"),value);
	if (curNPC)
	{
		curNPC->ResultEvent(value);
	}
}
void ATestPlayerController::SetCurNPCSelectUI(const int32& NPC_ID, const int32& State, const FString& Lang)
{
	int32 npc_id=NPC_ID;
	int32 npc_state=State;
	
	int32 FindKey =(npc_id*100)+(npc_state*10); // 시작하는 키값
	int32 count=0;
	TArray<FString> str;
	TArray<int32> result;

	// 110~115 까지 키값을 순회하기 => 선택지는 최대 5개까지니까 

	for (int i=FindKey ; i<=(FindKey+5); i++)
	{
		if (GI->NPCSelectMap.Contains(i)) // 있는 경우
		{
			// 위젯생성 하고 값 넣어주기 
			count++;
			const FNPCSelect& Select = GI->NPCSelectMap[i];
			result.Add(Select.Check);
			if (Lang == TEXT("kor"))
			{
				str.Add(*Select.Select_Kor);
			}
			else if(Lang == TEXT("eng"))
			{
				str.Add(*Select.Select_Eng);
			}
		}
		else // 없는경우
		{
			UE_LOG(LogTemp,Warning,TEXT("Select 를 찾을 수 없습니다."));
			break;
		}
	}

	// 한번호출
	PlayerHUD->NPCDialogueUI->CreateSelectionChildren(count,str,result);
}

