// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidget.h"

#include "SouvenirWidget.h"
#include "Components/Button.h"

void UNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Rule->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Rule);
	Btn_Souvenir->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Souvenir);
	Btn_Setting->OnClicked.AddDynamic(this,&UNoteWidget::OnClickBtn_Setting);
}

void UNoteWidget::OnClickBtn_Rule()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Hidden);
}

void UNoteWidget::OnClickBtn_Souvenir()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Visible);
}

void UNoteWidget::OnClickBtn_Setting()
{
	WBP_Souvenir->SetVisibility(ESlateVisibility::Hidden);
}
