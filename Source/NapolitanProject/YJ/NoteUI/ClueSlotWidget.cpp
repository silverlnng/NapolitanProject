// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueSlotWidget.h"

#include "ClueInfoWidget.h"
#include "NoteWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UClueSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TestPlayerController=GetOwningPlayer<ATestPlayerController>();
	PlayerHUD=TestPlayerController->GetHUD<APlayerHUD>();
	Btn_ClueSlot->OnHovered.AddDynamic(this,&UClueSlotWidget::OnClickBtn_ClueSlot);
	Btn_ClueSlot->OnClicked.AddDynamic(this,&UClueSlotWidget::OnClickBtn_ClueSlot);
}

void UClueSlotWidget::OnClickBtn_ClueSlot()
{
	PlayerHUD->NoteUI->WBP_ClueInfo->SetClueContent(Clue_ID);
}

void UClueSlotWidget::SetWidgetSwitcher(int32 val)
{
	WidgetSwitcher->SetActiveWidgetIndex(val);
}

void UClueSlotWidget::SetTextClueName(FString str)
{
	TextBlock_ClueName->SetText(FText::FromString(str));
}
