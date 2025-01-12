// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadConfirmWidget.h"

#include "SaveSlotSwitcherWidget.h"
#include "Components/Button.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

void ULoadConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Yes->OnClicked.AddDynamic(this,&ULoadConfirmWidget::OnClickYes);
	Btn_No->OnClicked.AddDynamic(this,&ULoadConfirmWidget::OnClickNo);
}

void ULoadConfirmWidget::OnClickYes()
{
	if (!SaveGameManager)
	{
		SaveGameManager = NewObject<UGameSaveController>(this);
	}
	SaveGameManager->LoadGameFromSlot(SaveSlotSwitcherWidget->SlotNumber);
	SetVisibility(ESlateVisibility::Hidden);
}

void ULoadConfirmWidget::OnClickNo()
{
	SetVisibility(ESlateVisibility::Hidden);
}
