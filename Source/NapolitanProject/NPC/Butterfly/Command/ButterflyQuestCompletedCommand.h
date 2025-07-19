// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
class ANPC_Butterfly;
/**
 * 
 */
class NAPOLITANPROJECT_API ButterflyQuestCompletedCommand : public IEventCommand
{
public:
	ButterflyQuestCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~ButterflyQuestCompletedCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ANPC_Butterfly* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;	
};
