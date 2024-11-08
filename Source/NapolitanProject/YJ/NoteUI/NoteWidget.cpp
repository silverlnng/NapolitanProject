// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidget.h"

#include "SouvenirWidget.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Rule->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_NPNInfo);
	Btn_Souvenir->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Souvenir);
	Btn_Setting->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Setting);
	WBP_Souvenir->SetVisibility(ESlateVisibility::Hidden);
	WBP_NPCInfo->SetVisibility(ESlateVisibility::Hidden);
	WBP_SettingInNote->SetVisibility(ESlateVisibility::Hidden);
}

void UNoteWidget::OnClickBtn_NPNInfo()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Hidden);
	WBP_NPCInfo->SetVisibility(ESlateVisibility::Visible);
	WBP_SettingInNote->SetVisibility(ESlateVisibility::Hidden);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Souvenir()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Visible);
	WBP_NPCInfo->SetVisibility(ESlateVisibility::Hidden);
	WBP_SettingInNote->SetVisibility(ESlateVisibility::Hidden);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}

void UNoteWidget::OnClickBtn_Setting()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Hidden);
	WBP_NPCInfo->SetVisibility(ESlateVisibility::Hidden);
	WBP_SettingInNote->SetVisibility(ESlateVisibility::Visible);
	if (ClickSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	}
}
