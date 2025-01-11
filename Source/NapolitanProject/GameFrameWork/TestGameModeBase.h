// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "TestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameModeBase();

	virtual void BeginPlay() override;

private:
	
};
