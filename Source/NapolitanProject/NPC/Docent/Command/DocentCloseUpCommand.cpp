// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentCloseUpCommand.h"

#include "../DocentV2.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

DocentCloseUpCommand::DocentCloseUpCommand(ATestPlayerController* INPC,ATestCharacter* INMainCharacter,UWorld* InWorld)
{
	PC=INPC;
	MainCharacter=INMainCharacter;
	World=InWorld;
}

DocentCloseUpCommand::~DocentCloseUpCommand()
{
}
void DocentCloseUpCommand::Execute()
{
	// 노트 UI 를 닫으면 실행하는 함수
	// 도슨트 조명키우고
	// 카메라 전환 안한상태에서 대사출력되도록 만들기 
	ADocentV2* DocentV2=nullptr;
	if (PC->curNPC)
	{
		DocentV2=Cast<ADocentV2>(PC->curNPC);
		if (DocentV2)
		{
			DocentV2->CloseUPCam();
			DocentV2->DocentLightOn();
			// 카메라 전환 안되도록 막기 
			// 컨트롤러 의  curNPC에 담아주기
			PC->curNPC =DocentV2;
			// TestPC 에서 대화창 시작하는 함수 시작하기
			PC->curNPC->playTalkAnimMontage();
			PC->StartNPCDialougueNoCamChange();
			PC->SetNPCDialougueText(0);
			// 나의 상태 변화
			MainCharacter->SetPlayerState(EPlayerState::Talking);
		}
	}
}