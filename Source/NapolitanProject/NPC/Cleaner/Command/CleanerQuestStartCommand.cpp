// Fill out your copyright notice in the Description page of Project Settings.


#include "CleanerQuestStartCommand.h"

#include "EngineUtils.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/Door_2Floor.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/Cleaner/NPC_Cleaner.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

CleanerQuestStartCommand::CleanerQuestStartCommand(ANPCCharacter* INNPC,ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	NPC=Cast<ANPC_Cleaner>(INNPC);
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

CleanerQuestStartCommand::~CleanerQuestStartCommand()
{
}

void CleanerQuestStartCommand::Execute()
{
	// 대화 창 닫고
	PC->StartEndNPCDialougue(false);

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
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Cleaner(1,1);
	},2.5f,false);

	

	FTimerHandle UITimer3;

	World->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FString QuestText =FString(TEXT("머리를 찾아주기"));
		PlayerHUD->InteractUI->AddQuestSlot(2,QuestText);
	},8.0f,false);

	FTimerHandle Timer4;

	World->GetTimerManager().SetTimer(Timer4,[this]()
	{
		// 2층문이 열리도록 하기
		for (TActorIterator<ADoor_2Floor> It(World, ADoor_2Floor::StaticClass()); It; ++It)
		{
			Door_2Floor=*It;
		}
		if(Door_2Floor)
		{
			Door_2Floor->UnBindBeginOverlap();
		}
	},14.0f,false);
}
