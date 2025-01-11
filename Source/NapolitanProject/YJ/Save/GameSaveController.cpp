// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveController.h"

#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UGameSaveController::SaveGameToSlot(int32 SlotIndex)
{
	ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController) return;

	ACharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;
	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* SaveGameInstance = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(UTestSaveGame::StaticClass()));
	
	if (SaveGameInstance)
	{
		// 데이터 저장 (예: 플레이어 위치)
		SaveGameInstance->PlayerLocation = PlayerCharacter->GetActorLocation(); 
	    SaveGameInstance->PlayerRotation =PlayerCharacter->GetActorRotation();   
		//SaveGameInstance->PlayerLevel = 5;
		//SaveGameInstance->GameTime = 123.45f;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Game saved to slot: %s"), *SlotName);
	}
}

UTestSaveGame* UGameSaveController::LoadGameFromSlot(int32 SlotIndex)
{
	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (LoadedGame)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Game loaded from slot: %s"), *SlotName);
		ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ACharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
		
		PlayerCharacter->SetActorLocation(LoadedGame->PlayerLocation);
		PlayerCharacter->SetActorRotation(LoadedGame->PlayerRotation);

		return LoadedGame;
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to load game from slot: %s"), *SlotName);
	return nullptr;
}


FString UGameSaveController::GetSlotName(int32 SlotIndex) const
{
	return FString::Printf(TEXT("%s%d"), *SlotPrefix, SlotIndex);
}