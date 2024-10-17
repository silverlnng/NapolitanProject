// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSaveController.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UGameSaveController : public UObject
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveGame(FVector PlayerPosition, FString LevelName, FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool LoadGame(FVector& OutPlayerPosition, FString& OutLevelName, FString SlotName);
};
