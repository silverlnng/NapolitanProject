// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidget.h"

#include "ClueInfoWidget.h"
#include "InventoryWidget.h"
#include "NPCInfoWidget.h"
#include "SouvenirWidget.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Rule->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_RuleInfo);
	Btn_Souvenir->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Souvenir);
	Btn_Setting->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Setting);
	Btn_ClueInfo->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_ClueInfo);
	Btn_Security->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Btn_Security);
	Btn_Cleaner->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Btn_Cleaner);
	Btn_Docent->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Btn_Docent);
	Btn_Oldman->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Btn_Oldman);
	
	Btn_Setting->SetVisibility(ESlateVisibility::Hidden);
	
	WBP_Inventory->SetVisibility(ESlateVisibility::Hidden);
}

void UNoteWidget::SelectContent(int32 idx)
{
	WS_Content->SetActiveWidgetIndex(idx);
}

void UNoteWidget::OnClickBtn_RuleInfo()
{
	SelectContent(0);
	
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Souvenir()
{
	SelectContent(4);
	
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Setting()
{
	SelectContent(3);

	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_ClueInfo()
{
	SelectContent(2);
	
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Btn_Docent()
{
	SelectContent(1);
	WBP_NPCInfo->SelectContent(2);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Btn_Security()
{
	SelectContent(1);
	WBP_NPCInfo->SelectContent(1);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Btn_Cleaner()
{
	SelectContent(1);
	WBP_NPCInfo->SelectContent(0);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Btn_Oldman()
{
	SelectContent(1);
	WBP_NPCInfo->SelectContent(3);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}
