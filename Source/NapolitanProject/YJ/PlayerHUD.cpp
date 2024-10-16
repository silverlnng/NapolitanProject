// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"
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
	

	if (!NYSFactory)
		return;

	NYSUi = CastChecked<UNYS_Choice>(CreateWidget(GetWorld(), NYSFactory));

	if (NYSUi) {
		NYSUi->AddToViewport();
		NYSUi->SetVisibility(ESlateVisibility::Hidden);
	}
	
}
