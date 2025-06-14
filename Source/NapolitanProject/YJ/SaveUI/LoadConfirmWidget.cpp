// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadConfirmWidget.h"

#include "SavedSlotWidget.h"
#include "SaveSlotSwitcherWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

void ULoadConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Yes->OnClicked.AddDynamic(this,&ULoadConfirmWidget::OnClickYes);
	Btn_No->OnClicked.AddDynamic(this,&ULoadConfirmWidget::OnClickNo);
	Btn_ReSave->OnClicked.AddDynamic(this,&ULoadConfirmWidget::OnClickReSave);
	
	GI=GetGameInstance<UMyTestGameInstance>();
	SaveGI= GI->GetSubsystem<USaveGISubsystem>();
	OnVisibilityChanged.AddDynamic(this,&ULoadConfirmWidget::HandleVisibilityChanged);
}

void ULoadConfirmWidget::OnClickYes()
{
	if (SaveGI&&SaveGI->GameSaveController)
	{
		SaveGI->GameSaveController->LoadGameFromSlot(SaveSlotSwitcherWidget->SlotNumber);
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}

void ULoadConfirmWidget::OnClickNo()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void ULoadConfirmWidget::OnClickReSave()
{
	if (SaveGI&&SaveGI->GameSaveController)
	{
		SaveGI->GameSaveController->SaveGameToSlot(SaveSlotSwitcherWidget->SlotNumber);
	}

	SaveSlotSwitcherWidget->SlotSwitcher->SetActiveWidgetIndex(0);
	// 저장하는 시간 구하기
	FDateTime Now = FDateTime::Now();

	// 날짜 및 시간 출력
	UE_LOG(LogTemp, Warning, TEXT("현재 날짜와 시간: %s"), *Now.ToString());

	// 특정 형식으로 출력
	FString FormattedTime = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	UE_LOG(LogTemp, Warning, TEXT("포맷된 시간: %s"), *FormattedTime);
	SaveSlotSwitcherWidget->WBP_SavedSlot->SetText_Date(FormattedTime);

	SaveSlotSwitcherWidget->WBP_SavedSlot->SetText_Loc(SaveLocation);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void ULoadConfirmWidget::HandleVisibilityChanged(ESlateVisibility InVisibility)
{
	// 이거 보이면 뒤에 슬롯들 클릭 안되도록 막아야함 .
	if (InVisibility == ESlateVisibility::Visible)
	{
		SlotClickProtection.Broadcast(true);
	}
	else if (InVisibility == ESlateVisibility::Hidden)
	{
		SlotClickProtection.Broadcast(false);
	}
}
