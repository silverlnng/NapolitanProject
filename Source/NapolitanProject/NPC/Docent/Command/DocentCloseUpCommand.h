// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ATestCharacter;
class ATestPlayerController;
/**
 * 
 */
class NAPOLITANPROJECT_API DocentCloseUpCommand: public IEventCommand
{
public:
	DocentCloseUpCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld);
	~DocentCloseUpCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
};
