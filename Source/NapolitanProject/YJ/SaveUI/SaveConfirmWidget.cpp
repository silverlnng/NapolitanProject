// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveConfirmWidget.h"

#include "SavedSlotWidget.h"
#include "SaveSlotSwitcherWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

void USaveConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//버튼에
	Btn_Yes->OnClicked.AddDynamic(this,&USaveConfirmWidget::OnClickYes);
	Btn_No->OnClicked.AddDynamic(this,&USaveConfirmWidget::OnClickNo);
}

void USaveConfirmWidget::OnClickYes()
{
	// 나자신을 생성한 위젯에 대한 정보를 알수있나 ??
	
	// SaveSlotSwitcherWidget 에 다가 저장하기
	// SaveSlotSwitcherWidget->SlotNumber 을 이용해서 Save하기
	if (!SaveGameManager)
	{
		SaveGameManager = NewObject<UGameSaveController>(this);
	}

	SaveGameManager->SaveGameToSlot(SaveSlotSwitcherWidget->SlotNumber);

	SaveSlotSwitcherWidget->SlotSwitcher->SetActiveWidgetIndex(0);
	// 저장하는 시간 구하기
	FDateTime Now = FDateTime::Now();

	// 날짜 및 시간 출력
	UE_LOG(LogTemp, Warning, TEXT("현재 날짜와 시간: %s"), *Now.ToString());

	// 특정 형식으로 출력
	FString FormattedTime = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	UE_LOG(LogTemp, Warning, TEXT("포맷된 시간: %s"), *FormattedTime);
	SaveSlotSwitcherWidget->WBP_SavedSlot->SetText_Date(FormattedTime);

	// 저장한 장소에 대해서 string으로 표현하기 
	SaveSlotSwitcherWidget->WBP_SavedSlot->SetText_Loc(SaveLocation);
	
	SetVisibility(ESlateVisibility::Hidden);
}

void USaveConfirmWidget::OnClickNo()
{
	SetVisibility(ESlateVisibility::Hidden);
}

