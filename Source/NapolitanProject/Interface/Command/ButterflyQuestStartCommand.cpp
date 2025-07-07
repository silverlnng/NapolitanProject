// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyQuestStartCommand.h"

#include "Engine/World.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"
#include "NapolitanProject/YJ/DialogueUI/NPCResultWidget.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"


ButterflyQuestStartCommand::ButterflyQuestStartCommand(ANPCCharacter* INNPC, ATestPlayerController* INPC,
	ATestCharacter* INMainCharacter, APlayerHUD* INPlayerHUD, UWorld* InWorld)
{
	NPC=Cast<ANPC_Butterfly>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

ButterflyQuestStartCommand::~ButterflyQuestStartCommand()
{
}

void ButterflyQuestStartCommand::Execute()
{
	PlayerHUD->NPCResultUI->SetVisibility(ESlateVisibility::Hidden);
	PlayerHUD->NPCResultUI->curOrder=0;
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
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Butterfly(1,1);
	},2.5f,false);
	

	FTimerHandle UITimer3;
	World->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FString QuestText =FString(TEXT("거미버거를 만들어주기"));
		PlayerHUD->InteractUI->AddQuestSlot(4,QuestText);
	},8.0f,false);
	
	FTimerHandle UITimer4;
	World->GetTimerManager().SetTimer(UITimer4,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
	},6.0f,false);
	
	if (NPC)
	{
		NPC->State=2; //현재 나비여서 state 증가시키기 
	}
}
