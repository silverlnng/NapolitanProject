// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHeadActor.h"

#include "EngineUtils.h"
#include "InteractWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
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
	
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);


	OnInventorySlot(); // 인벤작업
	GI->SavedItems.Add(this->GetClass());
	
	FTimerHandle UITimer4;
	
	GetWorld()->GetTimerManager().SetTimer(UITimer4,[this]()
	{
		FString QuestText =FString(TEXT("머리를 찾아주기"));
		PlayerHUD->InteractUI->RemoveQuestSlot(QuestText);
	},1.0f,false);
	

	FTimerHandle UITimer3;

	GetWorld()->GetTimerManager().SetTimer(UITimer3,[this]()
	{
		FString QuestText =FString(TEXT("머리를 가져다 주자"));
		PlayerHUD->InteractUI->AddQuestSlot(3,QuestText);
	},1.5f,false);

	
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

		// NPC_Cleaner가 다음 스테이트로 넘어간걸 게임저장을 해야함 
	}
}
