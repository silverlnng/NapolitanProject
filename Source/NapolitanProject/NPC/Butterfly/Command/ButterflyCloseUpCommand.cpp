// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyCloseUpCommand.h"

#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"

ButterflyCloseUpCommand::ButterflyCloseUpCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	World=InWorld;
}

ButterflyCloseUpCommand::~ButterflyCloseUpCommand()
{
}

void ButterflyCloseUpCommand::Execute()
{
	// 가까이 날라오다가 카메라에 부착되도록 만들기
	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(PC->curNPC);
	if (NPC_Butterfly)
	{
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("ButterflyCloseUpCommand")));
	}
	
}
