// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentDetectStartCommand.h"

#include "DocentV2.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

DocentDetectStartCommand::DocentDetectStartCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	World=InWorld;
}

DocentDetectStartCommand::~DocentDetectStartCommand()
{
}

void DocentDetectStartCommand::Execute()
{
	// 시간지연을 주기
	FTimerHandle UITimer2;
	World->GetTimerManager().SetTimer(UITimer2,[this]()
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
	},4.5f,false);
}
