// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"

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
	
	
}
