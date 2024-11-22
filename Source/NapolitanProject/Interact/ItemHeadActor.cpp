// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHeadActor.h"

#include "EngineUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/NPC/NPC_Cleaner.h"
#include "NapolitanProject/YJ/NoteUI/NoteWidget.h"
#include "NapolitanProject/YJ/NoteUI/NPCInfoWidget.h"

void AItemHeadActor::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ANPC_Cleaner> It(GetWorld(), ANPC_Cleaner::StaticClass()); It; ++It)
	{
		NPC_Cleaner = *It;
	}
}

void AItemHeadActor::OnPickup()
{
	CleanerNextState();
	Super::OnPickup();

	// 머리잡았을때 경비원의 노트ui 나오고 단서 추가하도록 하기
	MainCharacter->SetPlayerState(EPlayerState::UI);

	// 시간지연
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		PlayerHUD->NoteUI->SetVisibility(ESlateVisibility::Visible);

		PlayerHUD->NoteUI->OnClickBtn_Btn_Security();
		
	},2.0f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->NoteUI->WBP_NPCInfo->SetForcus_ScrollBox_Security(2,1);
	},2.5f,false);


	
}

void AItemHeadActor::PutDown()
{
	Super::PutDown();
}

void AItemHeadActor::CleanerNextState()
{
	if (NPC_Cleaner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NPC_Cleaner next state"));
		NPC_Cleaner->State=2;
	}
}
