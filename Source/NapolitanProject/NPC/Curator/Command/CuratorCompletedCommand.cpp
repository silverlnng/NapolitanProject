// Fill out your copyright notice in the Description page of Project Settings.


#include "CuratorCompletedCommand.h"

#include "NapolitanProject/NPC/Curator/ChaseStatue.h"

CuratorCompletedCommand::CuratorCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

CuratorCompletedCommand::~CuratorCompletedCommand()
{
}

void CuratorCompletedCommand::Execute()
{
	
}
