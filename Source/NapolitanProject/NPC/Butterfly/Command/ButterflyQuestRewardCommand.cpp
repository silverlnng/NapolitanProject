// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyQuestRewardCommand.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

ButterflyQuestRewardCommand::ButterflyQuestRewardCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,APlayerHUD* INPlayerHUD,UWorld* InWorld)
{
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
	
	
	// 스폰시키고
	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(PC->curNPC);
	if (NPC_Butterfly)
	{
		NPC_Butterfly->SpawnItems();
	}

	FTimerHandle DelayTimer;
	World->GetTimerManager().SetTimer(DelayTimer,[this]()
	{
		PC->StartEndNPCDialougue(false);
		MainCharacter->SetPlayerState(EPlayerState::UI);
	},4.5f,false);

	// 시간지연
	FTimerHandle UITimer;
	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Butterfly();
		
	},6.5f,false);

	FTimerHandle UITimer2;

	World->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Butterfly(2,3);
		
		PlayerHUD->NoteUI->State=EEventState::ButterflyEvent;
		
	},7.0f,false);

	// 여기서 이제 공중으로 날아가는 애니메이션 실행
	if (NPC_Butterfly)
	{
		NPC_Butterfly->PlayFlyHighMontage();
		// 콜리전 변경
		NPC_Butterfly->Cleared();
	}
	// 애니메이션 끝날때 가까이 날라오는거 연출
}
