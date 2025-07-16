// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyQuestRewardCommand.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

ButterflyQuestRewardCommand::ButterflyQuestRewardCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	NPC=Cast<ANPC_Butterfly>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

ButterflyQuestRewardCommand::~ButterflyQuestRewardCommand()
{
}

void ButterflyQuestRewardCommand::Execute()
{
	PC->StartEndNPCDialougue(false);
	
	// 스폰시키고
	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(PC->curNPC);
	if (NPC_Butterfly)
	{
		NPC_Butterfly->SpawnItems();
	}

	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Butterfly();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Butterfly(2,3);
	},2.5f,false);
}
