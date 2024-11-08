// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"
#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"

ATestGameModeBase::ATestGameModeBase()
{
	
}

void ATestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	const float AutoSaveInterval = 300.0f; // 자동 저장 주기 (300초)
	GetWorldTimerManager().SetTimer(AutoSaveTimerHandle, this, &ATestGameModeBase::AutoSaveGame, AutoSaveInterval, true);
	LoadLastSave();
}

void ATestGameModeBase::UpdateNPCClearState(const FString& NPCID, bool bIsCleared)
{
}

void ATestGameModeBase::AutoSaveGame()
{
}

void ATestGameModeBase::LoadLastSave()
{
}

void ATestGameModeBase::SaveGame()
{
}

UTestSaveGame* ATestGameModeBase::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		return Cast<UTestSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}
	return nullptr;
}

void ATestGameModeBase::InitializeNPCStates()
{
}

void ATestGameModeBase::RestoreNPCStates(const TMap<int32, int32>& LoadedNPCStates)
{
}
