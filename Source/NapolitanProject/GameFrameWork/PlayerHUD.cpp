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
#include "Components/ScrollBox.h"
#include "NapolitanProject/YJ/NoteUI/ClueInfoWidget.h"
#include "NapolitanProject/YJ/NoteUI/ClueSlotWidget.h"
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GI =GetGameInstance<UMyTestGameInstance>();
	
	NoteUI =CreateWidget<UNoteWidget>(GetWorld(),NoteWidgetFactory);
	if (NoteUI)
	{
		NoteUI->AddToViewport();
		NoteUI->SetVisibility(ESlateVisibility::Hidden);
	}

	// 초반에 slot 을 생성해주기
	
	NPCDialogueUI =CreateWidget<UNPCDialogueWidget>(GetWorld(),NPCDialogueWidgetFactory);
	if (NPCDialogueUI)
	{
		NPCDialogueUI->AddToViewport(1);
		NPCDialogueUI->SetVisibility(ESlateVisibility::Hidden);
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

	if (GI)
	{
		for (int i = 0; i < GI->ClueDataRowNames.Num(); i++)
		{
			// 겟수만큼 생성
			FClueData* ClueData =GI-> DT_Clue->FindRow<FClueData>(GI->ClueDataRowNames[i],TEXT(""));
			
			UClueSlotWidget* newSlot=CreateWidget<UClueSlotWidget>(GetWorld(),NoteUI->WBP_ClueInfo->ClueSlotWidgetFactory);

			 NoteUI->WBP_ClueInfo->ClueSlots.Add(i,newSlot);
			
			 newSlot->SetTextClueName(ClueData->Name);
			 newSlot->Clue_ID=i;
			 if (ClueData->Had)
			 {
				 // 있으면
			 	newSlot->SetWidgetSwitcher(1);
			 }
			 else
			 {
			 	// 없으면
			 	newSlot->SetWidgetSwitcher(0);
			 }
			 NoteUI->WBP_ClueInfo->ScrollBox_List->AddChild(newSlot);
			
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
