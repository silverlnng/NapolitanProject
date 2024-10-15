// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveController.h"

#include "Kismet/GameplayStatics.h"

void UGameSaveController::SaveGame(FVector PlayerPosition, FString LevelName, FString SlotName)
{
	//UGameSaveModel* SaveGameInstance = Cast<UGameSaveModel>(UGameplayStatics::CreateSaveGameObject(UGameSaveModel::StaticClass()));

	/*if (SaveGameInstance)
	{
		SaveGameInstance->PlayerPosition = PlayerPosition;
		SaveGameInstance->LevelName = LevelName;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
	}*/
}

bool UGameSaveController::LoadGame(FVector& OutPlayerPosition, FString& OutLevelName, FString SlotName)
{
	return true;
}
