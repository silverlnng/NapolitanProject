// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLevelGameModeBase.h"

#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestPlayerController.h"
#include "Blueprint/UserWidget.h"

void AStartLevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GI = GetGameInstance<UMyTestGameInstance>();
	
	OpeningUI =CreateWidget<UUserWidget>(GetWorld(),OpeningWidgetFactory);
	if (OpeningUI)
	{
		OpeningUI->AddToViewport();
	}
	
}
