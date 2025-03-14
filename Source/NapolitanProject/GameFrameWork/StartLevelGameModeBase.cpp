// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLevelGameModeBase.h"

#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestPlayerController.h"

void AStartLevelGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GI = GetGameInstance<UMyTestGameInstance>();
	
	PC = Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());

	PlayerHUD = PC->GetHUD<APlayerHUD>();

	
}
