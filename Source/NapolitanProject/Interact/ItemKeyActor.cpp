// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemKeyActor.h"

#include "EngineUtils.h"
#include "ExitDoor_First.h"

void AItemKeyActor::BeginPlay()
{
	Super::BeginPlay();
}

void AItemKeyActor::OnPickup()
{
	Super::OnPickup();
	// 탈출문 열릴수 있도록 만들기

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
