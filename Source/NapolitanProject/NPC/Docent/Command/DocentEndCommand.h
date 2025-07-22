// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ATestCharacter;
class ATestPlayerController;
class ADocentV2;
/**
 * 
 */
class NAPOLITANPROJECT_API DocentEndCommand: public IEventCommand
{
public:
	DocentEndCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld);
	~DocentEndCommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	ADocentV2* Docent;
};
