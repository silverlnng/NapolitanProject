// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentEndCommand.h"

#include "../DocentV2.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

DocentEndCommand::DocentEndCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld,UMyTestGameInstance* InGI)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	World=InWorld;
	GI=InGI;
}

DocentEndCommand::~DocentEndCommand()
{
}

void DocentEndCommand::Execute()
{
	if (PC->curNPC)
	{
		Docent=Cast<ADocentV2>(PC->curNPC);
	}

	FTimerHandle UITimer3;
	World->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FName eventKey =TEXT("DocentPlayEnd");
		if (GI->QuestCommandsMap.Contains(eventKey))
		{
			GI->QuestCommandsMap[eventKey].Done = true;
		}
	},1.0f,false);
	
	FTimerHandle UITimer2;
	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		// 대화 창 닫고
		PC->StartEndNPCDialougue(false);
		if (Docent)
		{
			Docent->DetachDestroy();
		}
	},3.5f,false);
	// 대사끝나면 카메라에서 떨어지고 점점 사라지도록 하기
	
}
