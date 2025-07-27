// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPC_Security;
class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
class UMyTestGameInstance;
/**
 * 
 */
class NAPOLITANPROJECT_API SecurityCompletedCommand: public IEventCommand
{
public:
	SecurityCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld,UMyTestGameInstance* InGI);
	~SecurityCompletedCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ANPC_Security* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;
	UMyTestGameInstance* GI;
};
