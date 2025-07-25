// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadScreenWidget.h"

#include "LoadConfirmWidget.h"
#include "SaveConfirmWidget.h"
#include "SavedSlotWidget.h"
#include "SaveSlotSwitcherWidget.h"
#include "VacantSaveWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void ULoadScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PC=GetOwningPlayer<ATestPlayerController>();
	
	Btn_Exit->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnClickedBtnExit);
	
	SaveConfirmWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_LoadConfirm->SetVisibility(ESlateVisibility::Hidden);
	
	SaveSlotSwitcherList.Add(0, SaveSlotSwitcherWidget_0);
	SaveSlotSwitcherList.Add(1, SaveSlotSwitcherWidget_1);
	SaveSlotSwitcherList.Add(2, SaveSlotSwitcherWidget_2);
	int index = 0;
	for (auto& i : SaveSlotSwitcherList)
	{
		i.Value->SlotNumber=index;
		i.Value->WBP_VacantSlot->SlotNumber=index;
		i.Value->WBP_SavedSlot->SlotNumber=index;
		index++;
	}
	
	
	SaveSlotSwitcherWidget_0->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnSaveQuestionWidgetCreate_0);
	SaveSlotSwitcherWidget_0->WBP_SavedSlot->Btn_LoadSelectSlot->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnLoadConfirm_0);

	SaveSlotSwitcherWidget_1->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnSaveQuestionWidgetCreate_1);
	SaveSlotSwitcherWidget_1->WBP_SavedSlot->Btn_LoadSelectSlot->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnLoadConfirm_1);

	SaveSlotSwitcherWidget_2->WBP_VacantSlot->Btn_SaveGame->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnSaveQuestionWidgetCreate_2);
	SaveSlotSwitcherWidget_2->WBP_SavedSlot->Btn_LoadSelectSlot->OnClicked.AddDynamic(this,&ULoadScreenWidget::OnLoadConfirm_2);

	OnVisibilityChanged.AddDynamic(this,&ULoadScreenWidget::HandleVisibilityChanged);
	GameSaveController = NewObject<UGameSaveController>(this);

	WBP_LoadConfirm->SlotClickProtection.AddDynamic(this, &ULoadScreenWidget::SlotClickProtection);
	SaveConfirmWidget->SlotClickProtection_.AddDynamic(this, &ULoadScreenWidget::SlotClickProtection);
}

void ULoadScreenWidget::HandleVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		SaveSlotInfos = GameSaveController->LoadAllSlotInfo(3);
		LoadUpdateUI(SaveSlotInfos);
	}
}


void ULoadScreenWidget::LoadUpdateUI(const TArray<UTestSaveGame*>& SlotInfos)
{
	//LoadGameInstance을  SlotInfos 으로 받아서 업데이트 하는 방법
	for (const UTestSaveGame* SaveGame:SlotInfos)
	{
		int index=SaveGame->SlotNum;
		SaveSlotSwitcherList[index]->SetActiveIndex(0);
		SaveSlotSwitcherList[index]->WBP_SavedSlot->SetText_Date(SaveGame->SaveTime);
		SaveSlotSwitcherList[index]->WBP_SavedSlot->SetText_Loc(SaveGame->SaveLocation);
	}
}

void ULoadScreenWidget::OnClickedBtnExit()
{
	SetVisibility(ESlateVisibility::Hidden);

	
	FString CurrentLevel=UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (CurrentLevel==TEXT("StartLevel")){return;}
	
	if (PC)
	{
		//현재 게임모드에 따라서 구별하기
		
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		TestCharacter = Cast<ATestCharacter>(PC->GetPawn());
		if (TestCharacter)
		{
			TestCharacter->SetPlayerState(EPlayerState::Idle);
		}
	}
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_0()
{
	UE_LOG(LogTemp, Warning, TEXT("VacantSlot Clicked"));
	
	SaveConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveConfirmWidget->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_0;
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_1()
{
	UE_LOG(LogTemp, Warning, TEXT("VacantSlot Clicked"));
	
	SaveConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveConfirmWidget->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_1;
}

void ULoadScreenWidget::OnSaveQuestionWidgetCreate_2()
{
	UE_LOG(LogTemp, Warning, TEXT("VacantSlot Clicked"));
	
	SaveConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveConfirmWidget->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_2;
}

void ULoadScreenWidget::OnLoadConfirm_0()
{
	WBP_LoadConfirm->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	WBP_LoadConfirm->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_0;
}

void ULoadScreenWidget::OnLoadConfirm_1()
{
	WBP_LoadConfirm->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	WBP_LoadConfirm->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_1;
}

void ULoadScreenWidget::OnLoadConfirm_2()
{
	WBP_LoadConfirm->SetVisibility(ESlateVisibility::Visible);
	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	WBP_LoadConfirm->SaveSlotSwitcherWidget = SaveSlotSwitcherWidget_2;
}

void ULoadScreenWidget::SlotClickProtection(bool val)
{
	if (val)
	{
		for (auto& i : SaveSlotSwitcherList)
		{
			i.Value->WBP_SavedSlot->Btn_LoadSelectSlot->SetIsEnabled(false);
			i.Value->WBP_VacantSlot->Btn_SaveGame->SetIsEnabled(false);
		}
	}
	else
	{
		for (auto& i : SaveSlotSwitcherList)
		{
			i.Value->WBP_SavedSlot->Btn_LoadSelectSlot->SetIsEnabled(true);
			i.Value->WBP_VacantSlot->Btn_SaveGame->SetIsEnabled(true);
		}
	}
	
}


