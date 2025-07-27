// Fill out your copyright notice in the Description page of Project Settings.


#include "CuratorUICommand.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"
#include "Components/SlateWrapperTypes.h"


CuratorUICommand::CuratorUICommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

CuratorUICommand::~CuratorUICommand()
{
}

void CuratorUICommand::Execute()
{
	PC->StartEndNPCDialougue(false);
	
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Curator();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Curator(2,2);
	},2.5f,false);
}
