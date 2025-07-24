// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interface/EventCommand.h"

class ANPCCharacter;
class APlayerHUD;
class ATestCharacter;
class ATestPlayerController;
class ADocentV2;
/**
 * 
 */
class NAPOLITANPROJECT_API DocentNoteUICommand: public IEventCommand
{
public:
	DocentNoteUICommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld);
	~DocentNoteUICommand();
	virtual void Execute() override;
private:
	UWorld* World;
	ATestPlayerController* PC;
	ATestCharacter* MainCharacter;
	APlayerHUD* PlayerHUD;
	ADocentV2* NPC_Docent;
};
