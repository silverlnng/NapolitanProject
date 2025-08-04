// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLevelGameModeBase.h"
#include "MyTestGameInstance.h"
#include "SaveGISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "NapolitanProject/YJ/StartUI/GameStartWidget.h"

void AStartLevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->SetShowMouseCursor(true);
	}
	
	GI =GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();

	StartWidgetUI =CreateWidget<UGameStartWidget>(GetWorld(),StartWidgetFactory);
	if (StartWidgetUI)
	{
		StartWidgetUI->AddToViewport(0);
		if (SaveGI)
		{
			StartWidgetUI->SaveGI=SaveGI;
		}
	}
}
