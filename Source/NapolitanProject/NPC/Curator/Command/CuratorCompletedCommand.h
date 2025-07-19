// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
class AChaseStatue;

/**
 * 
 */
class NAPOLITANPROJECT_API CuratorCompletedCommand: public IEventCommand
{
public:
	CuratorCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~CuratorCompletedCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	AChaseStatue* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;	
};
