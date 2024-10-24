// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerController.h"

#include "NapolitanProject/NPC/NPCCharacter.h"
#include "TestGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/SouvenirWidget.h"
#include "Components/Image.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/YJ/InteractWidget.h"
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

void ATestPlayerController::SetUIMode(bool value)
{
	if (!value)
	{
		
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
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

void ATestPlayerController::StartEndNPCDialougue(bool value)
{
	if (value) // 대화 시작할때 
	{
		SetUIMode(true); // ui 모드로 
		PlayerHUD->InteractUI->SetVisibleCrossHair(false);
		PlayerHUD->InteractUI->SetVisibleHBox(false);
		PlayerHUD->NPCDialogueUI->curOrder=0; // 초기화 작업 
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
		
		int32 npcID =curNPC->GetNPCID();
		UE_LOG(LogTemp,Warning,TEXT("%s,npcID : %d"),*CALLINFO,npcID);
		int32 npcState =curNPC->GetState();
		SetCurNPCSelectUI(npcID,npcState,"kor");
		
		SetNPCDialougueMaxSize();
	}
	else // 대화 끝날때 
	{
		PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
		PlayerHUD->NPCDialogueUI->curOrder=0; // 초기화 작업 
	}
}

void ATestPlayerController::SetNPCDialougueMaxSize()
{
	PlayerHUD->NPCDialogueUI->MaxOrder=0; // 초기화 작업
	
	int32 npcID =curNPC->GetNPCID();
	int32 npcState =curNPC->GetState();
	int32 FindKey =(npcID*1000)+(npcState*100);
	for (int i=FindKey ; i<(FindKey+100); i++)
	{
		if (GI->NPCDialogueMap.Contains(i))
		{
			PlayerHUD->NPCDialogueUI->MaxOrder++;
		}
		else
		{
			break;
		}
	}
}

// 최대값 설정해줘야함 
void ATestPlayerController::SetNPCDialougueText(int32 curOrder)
{
	//curOrder 을 받아서 상황처리 하기 
	UE_LOG(LogTemp,Warning,TEXT("%s,curOrder :%d"),*CALLINFO,curOrder);
	int32 npcID =curNPC->GetNPCID();
	UE_LOG(LogTemp,Warning,TEXT("%s,npcID : %d"),*CALLINFO,npcID);
	int32 npcState =curNPC->GetState();
	UE_LOG(LogTemp,Warning,TEXT("%s,npcState : %d"),*CALLINFO,npcState);
	int32 FindKey =(npcID*1000)+(npcState*100)+curOrder; // 시작하는 키값
	UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,FindKey);
	
	UE_LOG(LogTemp,Warning,TEXT("%s,MaxOrder : %d"),*CALLINFO,PlayerHUD->NPCDialogueUI->MaxOrder);
	
	////////// 버튼 보이게,안보이게 처리
	if (0==curOrder){PlayerHUD->NPCDialogueUI->Btn_Back->SetVisibility(ESlateVisibility::Hidden);}
	if (0!=curOrder){PlayerHUD->NPCDialogueUI->Btn_Back->SetVisibility(ESlateVisibility::Visible);}
	if (PlayerHUD->NPCDialogueUI->MaxOrder!=curOrder)
	{
		PlayerHUD->NPCDialogueUI->Btn_Next->SetVisibility(ESlateVisibility::Visible);
		// 선택창 나오도록 
		PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(false);
	}
	if (PlayerHUD->NPCDialogueUI->MaxOrder==curOrder)
	{
		PlayerHUD->NPCDialogueUI->Btn_Next->SetVisibility(ESlateVisibility::Hidden);

		// 선택창 나오도록 
		PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(true);
	}
	/////////////////////////////////////
	
	
	if (GI->NPCDialogueMap.Contains(FindKey)) // 있으면 출력하기 
	{
		FNPCDialogue Dialogue_=GI->NPCDialogueMap[FindKey];

		PlayerHUD->NPCDialogueUI->SetText_Dialogue(Dialogue_.Dialogue_Kor);
		
	}
	
	
}

void ATestPlayerController::SetCurNPC(ANPCCharacter* curNPC_)
{
	curNPC = curNPC_;
}



void ATestPlayerController::CallCurNPCResultEvent(int32 value)
{
	// 대화창 닫기 
	PlayerHUD->NPCDialogueUI->SetSelectSlotVisible(false);
	UE_LOG(LogTemp,Warning,TEXT("%s NPCResult :%d"),*CALLINFO,value);
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

