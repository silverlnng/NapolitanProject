// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPC_Security;
class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
/**
 * 
 */
class NAPOLITANPROJECT_API SecurityCompletedCommand: public IEventCommand
{
public:
	SecurityCompletedCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~SecurityCompletedCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ANPC_Security* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;
};
