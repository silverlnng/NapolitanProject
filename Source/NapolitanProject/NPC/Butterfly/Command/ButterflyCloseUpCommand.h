// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPCCharacter;
class ATestCharacter;
class ATestPlayerController;
/**
 * 
 */
class NAPOLITANPROJECT_API ButterflyCloseUpCommand: public IEventCommand
{
public:
	ButterflyCloseUpCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld);
	~ButterflyCloseUpCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
};
