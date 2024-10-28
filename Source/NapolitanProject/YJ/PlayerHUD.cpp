// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "DeadEndingWidget.h"
#include "InteractWidget.h"
#include "DialogueUI/NPCDialogueWidget.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
#include "YSEvanceUI.h"
#include "NoteUI/NoteWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
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
	

	if (!YSEvanceFactory)
		return;

	YsEvanceUserWidget = CastChecked<UYSEvanceUI>(CreateWidget(GetWorld(), YSEvanceFactory));

	if (YsEvanceUserWidget) {
		YsEvanceUserWidget->AddToViewport();
		YsEvanceUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	

	
}
