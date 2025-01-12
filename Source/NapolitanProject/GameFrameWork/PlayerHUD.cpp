// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "MyTestGameInstance.h"
#include "../YJ/DeadEndingWidget.h"
#include "../Interact/InteractWidget.h"
#include "../YJ/DialogueUI/NPCDialogueWidget.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "YSEvanceUI.h"
#include "../YJ/NoteUI/NoteWidget.h"
#include "Animation/WidgetAnimation.h"
#include "BehaviorTree/Tasks/BTTask_PlayAnimation.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueInfoWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueSlotWidget.h"
#include "NapolitanProject/YJ/SaveUI/LoadScreenWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GI =GetGameInstance<UMyTestGameInstance>();
	
	NoteUI =CreateWidget<UNoteWidget>(GetWorld(),NoteWidgetFactory);
	if (NoteUI)
	{
		NoteUI->AddToViewport(2);
		NoteUI->SetVisibility(ESlateVisibility::Hidden);
	}

	// 초반에 slot 을 생성해주기
	
	NPCDialogueUI =CreateWidget<UNPCDialogueWidget>(GetWorld(),NPCDialogueWidgetFactory);
	if (NPCDialogueUI)
	{
		NPCDialogueUI->AddToViewport(1);
		NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
	}

	NPCResultUI =CreateWidget<UNPCResultWidget>(GetWorld(),NPCResultWidgetFactory);
	if (NPCResultUI)
	{
		NPCResultUI->AddToViewport(1);
		NPCResultUI->SetVisibility(ESlateVisibility::Hidden);
	}

	
	InteractUI=CreateWidget<UInteractWidget>(GetWorld(),InteractWidgetFactory);
	if (InteractUI)
	{
		InteractUI->AddToViewport();
		InteractUI->SetVisibleCrossHair(true);
		InteractUI->SetVisibleHBox(false);
	}

	DeadEndingWidgetUI=CreateWidget<UDeadEndingWidget>(GetWorld(),DeadEndingWidgetFactory);
	if (DeadEndingWidgetUI)
	{
		DeadEndingWidgetUI->AddToViewport();
		DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	MonolugueWidgetUI =CreateWidget<UMonolugueWidget>(GetWorld(),MonolugueWidgetFactory);
	if (MonolugueWidgetUI)
	{
		MonolugueWidgetUI->AddToViewport(1);
		MonolugueWidgetUI->SetVisibility(ESlateVisibility::Hidden);
	}
	
	LoadScreenUI=CreateWidget<ULoadScreenWidget>(GetWorld(),LoadScreenWidgetFactory);
	if (LoadScreenUI)
	{
		LoadScreenUI->AddToViewport(1);
		LoadScreenUI->SetVisibility(ESlateVisibility::Hidden);
		if (GI)
		{
			LoadScreenUI->LoadUpdateUI(GI->SaveSlotInfos);
		}
	}

	if (GI)
	{
		for (int i = 0; i < GI->ClueDataRowNames.Num(); i++)
		{
			// 겟수만큼 생성
			FClueData* ClueData =GI-> DT_Clue->FindRow<FClueData>(GI->ClueDataRowNames[i],TEXT(""));
			
			UClueSlotWidget* newSlot=CreateWidget<UClueSlotWidget>(GetWorld(),NoteUI->WBP_ClueInfo->ClueSlotWidgetFactory);
			
			 NoteUI->WBP_ClueInfo->ScrollBox_List->AddChild(newSlot);
			
			 NoteUI->WBP_ClueInfo->ClueSlots.Add(i,newSlot);
			
			 newSlot->SetTextClueName(ClueData->Name);
			 newSlot->Clue_ID=ClueData->ClueID;
			
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*ClueData->Name);
			 if (ClueData->Had)
			 {
				 // 있으면
			 	//newSlot->SetWidgetSwitcher(1);
			 	newSlot->Btn_ClueSlot->SetIsEnabled(true);
			 	newSlot->WidgetSwitcher->SetActiveWidgetIndex(1);
			 }
			 else
			 {
			 	// 없으면
			 	newSlot->WidgetSwitcher->SetActiveWidgetIndex(0);
			 }
			
			
		}
	}
	
	
}

void APlayerHUD::CreateYSEvance()
{
	if (!YSEvanceFactory)
		return;
	
	YsEvanceUserWidget = CreateWidget<UYSEvanceUI>(GetWorld(), YSEvanceFactory);
	
	if (YsEvanceUserWidget) {
		YsEvanceUserWidget->AddToViewport();
		YsEvanceUserWidget->SetVisibility(ESlateVisibility::Visible);
		//텍스트 애니메이션
		YsEvanceUserWidget->PlayAnim();
	}
}

void APlayerHUD::UpdateClueSlotWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateClueSlotWidget"));
	TArray<FName> RowNames = GI->DT_Clue->GetRowNames();

	for (int i = 0; i < GI->ClueDataRowNames.Num(); i++)
	{
		FClueData* ClueData =GI-> DT_Clue->FindRow<FClueData>(GI->ClueDataRowNames[i],TEXT(""));
		if (ClueData->Had)
		{
			UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*ClueData->Name);
			UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,i);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->SetWidgetSwitcher(1);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->Btn_ClueSlot->SetIsEnabled(true);
			NoteUI->WBP_ClueInfo->ClueSlots[i]->WidgetSwitcher->SetActiveWidgetIndex(1);
		}
	}		
}
