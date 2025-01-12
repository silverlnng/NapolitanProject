// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueSlotWidget.h"

#include "ClueInfoWidget.h"
#include "NoteWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UClueSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TestPlayerController=GetOwningPlayer<ATestPlayerController>();
	PlayerHUD=TestPlayerController->GetHUD<APlayerHUD>();
	Btn_ClueSlot->OnHovered.AddDynamic(this,&UClueSlotWidget::OnClickBtn_ClueSlot);
	Btn_ClueSlot->OnClicked.AddDynamic(this,&UClueSlotWidget::OnClickBtn_ClueSlot);
	Btn_ClueSlot->SetIsEnabled(false);
}

void UClueSlotWidget::OnClickBtn_ClueSlot()
{
	PlayerHUD->NoteUI->WBP_ClueInfo->SetClueContent(Clue_ID);
}

void UClueSlotWidget::SetWidgetSwitcher(int32 val)
{
	//버튼 활성화
	Btn_ClueSlot->SetIsEnabled(true);
	WidgetSwitcher->SetActiveWidgetIndex(val);
	UE_LOG(LogTemp,Warning,TEXT("%s"),*CALLINFO);
}

void UClueSlotWidget::SetTextClueName(FString str)
{
	TextBlock_ClueName->SetText(FText::FromString(str));
}
