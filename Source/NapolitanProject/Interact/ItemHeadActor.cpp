// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHeadActor.h"

#include "EngineUtils.h"
#include "NapolitanProject/NPC/NPC_Cleaner.h"

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
