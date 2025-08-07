// Fill out your copyright notice in the Description page of Project Settings.


#include "CuratorCompletedCommand.h"

#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Curator/ChaseStatue.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

CuratorCompletedCommand::CuratorCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld,UMyTestGameInstance* InGI)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
	GI=InGI;
}

CuratorCompletedCommand::~CuratorCompletedCommand()
{
}

void CuratorCompletedCommand::Execute()
{
	FName eventKey =TEXT("CuratorCompleted");
	if (GI->QuestCommandsMap.Contains(eventKey))
	{
		GI->QuestCommandsMap[eventKey].Done = true;
	}
	// 큐레이터와의 만남 후 체크 되는것.
		// 해바라기 그림문에서 체크하기 
}
