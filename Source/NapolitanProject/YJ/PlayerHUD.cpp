// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "NYS_Choice.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryUI =CreateWidget<UInventoryWidget>(GetWorld(),InventoryFactory);
	if (InventoryUI)
	{
		InventoryUI->AddToViewport();
		InventoryUI->SetVisibility(ESlateVisibility::Hidden);
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
