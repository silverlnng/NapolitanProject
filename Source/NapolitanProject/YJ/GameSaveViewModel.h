// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NAPOLITANPROJECT_API GameSaveViewModel
{
public:
	GameSaveViewModel();
	~GameSaveViewModel();

public:
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveGame(FVector PlayerPosition, FString LevelName);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool LoadGame(FVector& OutPlayerPosition, FString& OutLevelName);
	
};
