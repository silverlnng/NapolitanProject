// Fill out your copyright notice in the Description page of Project Settings.


#include "Souvenir_Docent.h"

#include "InteractWidget.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/EventComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/Docent/DocentV2.h"
#include "NapolitanProject/YJ/Monologue/MonologueTriggerBox.h"

void ASouvenir_Docent::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 ASouvenir_Docent::GetSouvenirID()
{
	return this->SouvenirID;
}

FString ASouvenir_Docent::GetSouvenirName()
{
	return this->SouvenirName;
}

void ASouvenir_Docent::OnPickup()
{
	Super::OnPickup();

	// 시간 지연 되면 서
	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		PlayerHUD->InteractUI->PlayNoteUIEvent(true);
		if (TestPC)
		{
			TestPC->EventComponent->Event_Docent_NoteUI();
		}
	},2.0f,false);

	// 도슨트 ADocentV2 를 멈추고 , 그림속으로 들어가도록 만들기
	if (Docent)
	{
		Docent->PickUPNote();
	}

	// 독백박스 가 사라지도록 만들기
	if (MonologueBox)
	{
		MonologueBox->Destroy();
	}
}

void ASouvenir_Docent::ChangeCollResponseIgnore()
{
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Ignore);
}

void ASouvenir_Docent::ChangeCollResponseBlock()
{
	BoxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Block);
}
