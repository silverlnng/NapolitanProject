// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanerQuestCompletedCommand.h"

#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/Cleaner/NPC_Cleaner.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

CleanerQuestCompletedCommand::CleanerQuestCompletedCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	NPC=Cast<ANPC_Cleaner>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

CleanerQuestCompletedCommand::~CleanerQuestCompletedCommand()
{
}

void CleanerQuestCompletedCommand::Execute()
{
	// 대화 창 닫고
	PC->StartEndNPCDialougue(false);
	PC->EndResult();
	MainCharacter->SetPlayerState(EPlayerState::UI);
	
	// 시간지연
	FTimerHandle UITimer;

	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Cleaner();
		
	},2.0f,false);
	

	// 시간지연
	FTimerHandle UITimer2;

	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Cleaner(2,2);
	},2.5f,false);

	FTimerHandle UITimer3;

	World->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
		
	},6.0f,false);

	FTimerHandle UITimer4;
	
	World->GetTimerManager().SetTimer(UITimer4,[this]()
	{
		FString QuestText =FString(TEXT("머리를 가져다 주자"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},8.0f,false);
}
