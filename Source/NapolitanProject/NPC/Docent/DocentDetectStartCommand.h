// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"
class ANPCCharacter;
class ATestCharacter;
class ATestPlayerController;
class ADocentV2;
/**
 * 
 */
class NAPOLITANPROJECT_API DocentDetectStartCommand: public IEventCommand
{
public:
	DocentDetectStartCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter);
	~DocentDetectStartCommand();
	virtual void Execute() override;
private:
	ADocentV2* NPC;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
};
