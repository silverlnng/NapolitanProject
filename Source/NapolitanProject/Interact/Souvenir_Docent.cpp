// Fill out your copyright notice in the Description page of Project Settings.


#include "Souvenir_Docent.h"

#include "InteractWidget.h"
#include "NapolitanProject/GameFrameWork/EventComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

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
	
	// 미술관을 탐색하자 퀘스트 발생 시키기
	/*FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this]()
	{
		FString QuestText =FString(TEXT("미술관을 탐색하자"));
		PlayerHUD->InteractUI->AddQuestSlot(1,QuestText);
	},6.0f,false);*/

	// 도슨트 ADocentV2 를 멈추고 , 그림속으로 들어가도록 만들기 
}
