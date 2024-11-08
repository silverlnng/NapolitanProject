// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDongjon.h"

#include "../GameFrameWork/TestPlayerController.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/YJ/DialogueUI/NPCDialogueWidget.h"

// Sets default values
ANPCDongjon::ANPCDongjon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPCDongjon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCDongjon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCDongjon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCDongjon::Interact()
{
	Super::Interact();
}

int32 ANPCDongjon::GetNPCID()
{
	return NPC_ID;
}

int32 ANPCDongjon::GetState()
{
	return State;
}

void ANPCDongjon::ResultEvent(int32 result)
{
	// State 와 선택지의  result 에 따라 이벤트 정의하기
	if(1==State)
	{
		if(0 == result)
		{
			//플레이어 카메라 전환 -> 노인을 바라보도록 카메라를 다시 조정
			
			//“큐레이터에게 가보십시오. 분명 도움이 되리라.” 라는 대사
			//결과 대사 출력 부분
			int32 key=(NPC_ID*100)+(State*10)+result;
			PlayerHUD->NPCDialogueUI->SetVisibility(ESlateVisibility::Visible);
			TestPC->SetCurNPCResultUI(key);
			
		}
	}
}
