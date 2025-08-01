// Fill out your copyright notice in the Description page of Project Settings.


#include "ButterflyQuestCompletedCommand.h"

#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/Butterfly/NPC_Butterfly.h"

ButterflyQuestCompletedCommand::ButterflyQuestCompletedCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter, APlayerHUD* INPlayerHUD, UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	PlayerHUD=INPlayerHUD;
	World=InWorld;
}

void ButterflyQuestCompletedCommand::Execute()
{
	// 카메라 나비 껄로 유지하기 
	
	FTimerHandle UITimer;
	World->GetTimerManager().SetTimer(UITimer,[this]()
	{
		FString QuestText =FString(TEXT("거미버거를 만들어주기"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},1.0f,false);

	// 여기서 먹는 애니메이션 실행하기
	ANPC_Butterfly* NPC_Butterfly= Cast<ANPC_Butterfly>(PC->curNPC);
	if (NPC_Butterfly)
	{
		NPC_Butterfly->PlayEatMontage();
	}
}

ButterflyQuestCompletedCommand::~ButterflyQuestCompletedCommand()
{
}
