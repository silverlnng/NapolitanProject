// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLevelGameModeBase.h"
#include "PlayerHUD.h"
#include "TestPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/YJ/StartUI/GameStartWidget.h"

void AStartLevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PC=Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeUIOnly());
	PC->SetShowMouseCursor(true);
	
	PlayerHUD =PC->GetHUD<APlayerHUD>();

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,[this]()
	{
		//퀘스트 UI 도 안보이도록 !
		if (PlayerHUD&&PlayerHUD->InteractUI)
		{
			PlayerHUD->InteractUI->SetVisibility(ESlateVisibility::Hidden);
		}
	},0.5f,false);
	
	StartWidgetUI =CreateWidget<UGameStartWidget>(GetWorld(),StartWidgetFactory);
	if (StartWidgetUI)
	{
		StartWidgetUI->AddToViewport(0);
	}
}
