// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentDetectStartCommand.h"

#include "DocentV2.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

DocentDetectStartCommand::DocentDetectStartCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter)
{
	NPC=Cast<ADocentV2>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;

}

DocentDetectStartCommand::~DocentDetectStartCommand()
{
}

void DocentDetectStartCommand::Execute()
{
	// 대화 창 닫고
	PC->StartEndNPCDialougue(false);
	
	// 도슨트 탐색 시작하도록
	ADocentV2* DocentV2=nullptr;
	if (PC->curNPC)
	{
		DocentV2=Cast<ADocentV2>(PC->curNPC);
		if (DocentV2)
		{
			DocentV2->bOnlyOnce=true;
			DocentV2->InMaxDetectionDistance=false;
		}
	}
}
