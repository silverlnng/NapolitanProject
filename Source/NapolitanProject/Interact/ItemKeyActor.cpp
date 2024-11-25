// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemKeyActor.h"

#include "EngineUtils.h"
#include "ExitDoor_First.h"
#include "InteractWidget.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"

void AItemKeyActor::BeginPlay()
{
	Super::BeginPlay();
}

void AItemKeyActor::OnPickup()
{
	Super::OnPickup();
	// 탈출문 열릴수 있도록 만들기

	// 열쇠를 획득했다 여기서 나오도록 하기
	FTimerHandle SouvenirTimer1;
	GetWorldTimerManager().SetTimer(SouvenirTimer1, [this]()
	{
		FString KeyText =FString(TEXT("열쇠"));
		PlayerHUD->InteractUI->GetSouvenirEvent(KeyText);
	}, 0.5f, false);


	AExitDoor_First* Door=nullptr;
	for (TActorIterator<AExitDoor_First> It(GetWorld(), AExitDoor_First::StaticClass()); It; ++It)
	{
		Door = *It;
	}
	if (Door)
	{
		Door->BindBeginOverlap();
	}
}
