// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameModel.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USaveGameModel : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector LastPlayerPosition;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString LastLevelName;
	
};
