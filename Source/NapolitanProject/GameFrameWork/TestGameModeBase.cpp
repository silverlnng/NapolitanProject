// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"

#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"

ATestGameModeBase::ATestGameModeBase()
{
	
}

void ATestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Current Game Mode: %s"), *GetWorld()->GetAuthGameMode()->GetName());
	
	PC=Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	
	MainCharacter=Cast<ATestCharacter>(PC->GetPawn());

	PlayerHUD =PC->GetHUD<APlayerHUD>();
	
	GI =GetGameInstance<UMyTestGameInstance>();
	if (GI)
	{
		// 시간 지연을 주기

		FTimerHandle GITimer;

		GetWorld()->GetTimerManager().SetTimer(GITimer,[this]()
		{
			GI->RestoreAttachedItems();
		},2.0f,false);
	}
	
}

