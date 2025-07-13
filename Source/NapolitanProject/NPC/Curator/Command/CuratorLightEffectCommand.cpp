// Fill out your copyright notice in the Description page of Project Settings.


#include "CuratorLightEffectCommand.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Curator/ChaseStatue.h"

CuratorLightEffectCommand::CuratorLightEffectCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	NPC=Cast<AChaseStatue>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

CuratorLightEffectCommand::~CuratorLightEffectCommand()
{
	
}

void CuratorLightEffectCommand::Execute()
{
	// 조명 깜빡임 효과를 주기
	AChaseStatue* Curator=nullptr;
	if (PC->curNPC)
	{
		Curator=Cast<AChaseStatue>(PC->curNPC);
		if (Curator)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Curator->LightEffect")));
			Curator->LightEffect();
		}
	}
}
