// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyQuestCompletedCommand.h"

#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"

ButterflyQuestCompletedCommand::ButterflyQuestCompletedCommand(ANPCCharacter* INNPC, ATestPlayerController* INPC,
                                                               ATestCharacter* INMainCharacter, APlayerHUD* INPlayerHUD, UWorld* InWorld)
{
	NPC=Cast<ANPC_Butterfly>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

void ButterflyQuestCompletedCommand::Execute()
{
	FTimerHandle UITimer;

	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		FString QuestText =FString(TEXT("거미버거를 만들어주기"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},1.0f,false);
}

ButterflyQuestCompletedCommand::~ButterflyQuestCompletedCommand()
{
}
