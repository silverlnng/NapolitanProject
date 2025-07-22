// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentNoteUICommand.h"

#include "../DocentV2.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

DocentNoteUICommand::DocentNoteUICommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

DocentNoteUICommand::~DocentNoteUICommand()
{
}

void DocentNoteUICommand::Execute()
{
	PC->StartEndNPCDialougue(false);
	PC->EndResult();
	MainCharacter->SetPlayerState(EPlayerState::UI);

	//여기서 도슨트도 가까이 오도록 만들기
	NPC_Docent= Cast<ADocentV2>(PC->curNPC);
	// 월드에서 도슨트 찾기 
	// 시간지연
	FTimerHandle UITimer;

	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Docent();

		PlayerHUD->NoteUI->State=EEventState::DocentEvent;
		
	},1.0f,false);

	FTimerHandle UITimer2;

	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Docent(2,1);
	},1.5f,false);
	
}
