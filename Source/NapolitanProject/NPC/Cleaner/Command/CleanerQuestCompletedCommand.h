// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPC_Cleaner;
class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
/**
 * 
 */
class NAPOLITANPROJECT_API CleanerQuestCompletedCommand: public IEventCommand
{
public:
	CleanerQuestCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~CleanerQuestCompletedCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ANPC_Cleaner* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;
};
