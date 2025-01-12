// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveController.h"

#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UGameSaveController::SaveGameToSlot(int32 SlotIndex)
{
	ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController) return;

	ACharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;
	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* SaveGameInstance = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(UTestSaveGame::StaticClass()));
	
	if (SaveGameInstance)
	{
		SaveGameInstance->SlotNum=SlotIndex;
		// 데이터 저장 (예: 플레이어 위치)
		SaveGameInstance->PlayerLocation = PlayerCharacter->GetActorLocation(); 
	    SaveGameInstance->PlayerRotation =PlayerCharacter->GetActorRotation();
		
		// 저장한 날짜 
		FDateTime Now = FDateTime::Now();
		SaveGameInstance->SaveTime = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
		
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Game saved to slot: %s"), *SlotName);
	}
}

UTestSaveGame* UGameSaveController::LoadGameFromSlot(int32 SlotIndex)
{
	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (LoadedGame)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Game loaded from slot: %s"), *SlotName);
		ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
			if (PlayerCharacter)
			{
				PlayerCharacter->SetActorLocation(LoadedGame->PlayerLocation);
				PlayerCharacter->SetActorRotation(LoadedGame->PlayerRotation);
				
			}
		}
		

		return LoadedGame;
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to load game from slot: %s"), *SlotName);
	return nullptr;
}


FString UGameSaveController::GetSlotName(int32 SlotIndex) const
{
	return FString::Printf(TEXT("%s%d"), *SlotPrefix, SlotIndex);
}

TArray<UTestSaveGame*> UGameSaveController::LoadAllSlotInfo(int32 MaxSlots)
{
	TArray<UTestSaveGame*> SlotInfos;
	
	for (int32 SlotIndex = 0; SlotIndex < MaxSlots; ++SlotIndex)
	{
		FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);

		// 슬롯 데이터 로드
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			UTestSaveGame* LoadGameInstance = Cast<UTestSaveGame>(
				UGameplayStatics::LoadGameFromSlot(SlotName, 0));

			if (LoadGameInstance)
			{
				FString SlotInfo = FString::Printf(TEXT("슬롯 %d: 저장 시간 %s"), 
					SlotIndex, *LoadGameInstance->SaveTime);
				
				SlotInfos.Add(LoadGameInstance);
				// ui 를 업데이트
				
				UE_LOG(LogTemp, Warning, TEXT("%s"), *SlotInfo);
			}
		}
		else // 없는 경우 
		{
			// 비어 있는 슬롯 정보 추가
			//FString EmptySlotInfo = FString::Printf(TEXT("슬롯 %d: 저장 데이터 없음"), SlotIndex);
			//SlotInfos.Add(EmptySlotInfo);
		}
	}

	return SlotInfos;
}