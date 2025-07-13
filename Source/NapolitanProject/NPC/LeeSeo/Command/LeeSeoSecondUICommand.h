// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
class ANPC_LeeSeo;
/**
 * 
 */
class NAPOLITANPROJECT_API LeeSeoSecondUICommand: public IEventCommand
{
public:
	LeeSeoSecondUICommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~LeeSeoSecondUICommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ANPC_LeeSeo* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;	
};
