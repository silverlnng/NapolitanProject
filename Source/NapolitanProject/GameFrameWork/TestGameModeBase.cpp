// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"

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
	/*if (PC)
	{
		TestCharacter = Cast<ATestCharacter>(PC->GetPawn());
		if (TestCharacter)
		{
			TestCharacter->SetPlayerState(EPlayerState::Idle);
		}
	}*/
}

