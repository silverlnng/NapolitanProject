// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveController.h"

#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UGameSaveController::SaveGameToSlot(int32 SlotIndex)
{
	ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController) return;

	ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;

	UMyTestGameInstance* GameInstance = Cast<UMyTestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* SaveGameInstance = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(UTestSaveGame::StaticClass()));
	
	if (SaveGameInstance)
	{
		SaveGameInstance->SlotNum=SlotIndex;
		// 데이터 저장 (예: 플레이어 위치)
		//SaveGameInstance->PlayerLocation = PlayerCharacter->GetActorLocation(); 
	   //SaveGameInstance->PlayerRotation =PlayerCharacter->GetActorRotation();

		SaveGameInstance->PlayerLocation = PlayerCharacter->SaveTransform.GetLocation();
		SaveGameInstance->PlayerRotation=PlayerCharacter->SaveTransform.GetRotation().Rotator();
		
		SaveGameInstance->PlayerLevel=UGameplayStatics::GetCurrentLevelName(GetWorld());
		// 저장한 날짜 
		FDateTime Now = FDateTime::Now();
		SaveGameInstance->SaveTime = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
		SaveGameInstance->DateTime=Now;

		// 저장한 위치
		SaveGameInstance->SaveLocation=PlayerCharacter->SaveLocationStr;
		
		// 획득한 단서에 대해 저장하기
		if (GameInstance && GameInstance->DT_Clue)
		{
			TArray<FName> RowNames = GameInstance->DT_Clue->GetRowNames();

			for (const FName& RowName : RowNames)
			{
				FClueData* Row = GameInstance->DT_Clue->FindRow<FClueData>(RowName, "");
				if (Row)
				{
					// 상태 저장
					SaveGameInstance->ClueStates.Add(RowName,Row->Had);
					UE_LOG(LogTemp, Warning, TEXT("Game saved to slot clue: %s"), *Row->Name);
				}
			}

			// 클리어한 npc 정보를 저장하기
			if (!GameInstance->ClearedNPC.IsEmpty())
			{
				SaveGameInstance->ClearedNPC=GameInstance->ClearedNPC;
			}

			if (!GameInstance->NPCEventManage.IsEmpty())
			{
				SaveGameInstance->NPCEventManage=GameInstance->NPCEventManage;
			}
			
			if (!GameInstance->QuestSlots.IsEmpty())
			{
				SaveGameInstance->QuestSlots=GameInstance->QuestSlots;
			}

			if (!GameInstance->AcquireSouvenir.IsEmpty())
			{
				SaveGameInstance->AcquireSouvenir=GameInstance->AcquireSouvenir;
			}
			
		}

		// 슬롯에 저장
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
		
		UE_LOG(LogTemp, Warning, TEXT("Game saved to slot: %s"), *SlotName);
		UE_LOG(LogTemp, Warning, TEXT("Game saved to slot: %s"), *UGameplayStatics::GetCurrentLevelName(GetWorld()));
	}
}

UTestSaveGame* UGameSaveController::LoadGameFromSlot(int32 SlotIndex)
{
	////// 슬롯 불러오기 ///////////////
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* LoadedGame = Cast<UTestSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	//////////////////////////////////
	
	if (LoadedGame)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Game loaded from slot: %s"), *SlotName);
		UGameplayStatics::OpenLevel(GetWorld(),FName(*LoadedGame->PlayerLevel));
		
		UMyTestGameInstance* GameInstance = Cast<UMyTestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		
		if (GameInstance)
		{
			if (GameInstance->DT_Clue)
			{
				TMap<FName, bool>& SavedClueStates = LoadedGame->ClueStates;

				for (const TPair<FName, bool>& Pair : SavedClueStates)
				{
					FClueData* Row = GameInstance->DT_Clue->FindRow<FClueData>(Pair.Key, "");
					if (Row)
					{
						// 상태 복원
						Row->Had = Pair.Value;
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("ClueStates 로드 완료"));
			}
		

			//// 클리어한 npc 정보를 로드해서 전달하기
			if (!LoadedGame->ClearedNPC.IsEmpty())
			{

				for (int32 &val:LoadedGame->ClearedNPC)
				{
					UE_LOG(LogTemp, Warning, TEXT("LoadedGame->ClearedNPC %s,%d"),*CALLINFO,val);
				}
				
				GameInstance->ClearedNPC=LoadedGame->ClearedNPC;
				for (int32 &val:GameInstance->ClearedNPC)
				{
					UE_LOG(LogTemp, Warning, TEXT("GameInstance->ClearedNPC %s,%d"),*CALLINFO,val);
					//제대로 되는지 로그로 확인하기

					//그리고 클리어한 npc는 레벨에 나오지 않도록 작업해야함 . 
				}
			}

			if (!LoadedGame->NPCEventManage.IsEmpty())
			{
				GameInstance->NPCEventManage=LoadedGame->NPCEventManage;
			}
			
			if (!LoadedGame->QuestSlots.IsEmpty())
			{
				GameInstance->QuestSlots=LoadedGame->QuestSlots;
			}

			if (!LoadedGame->AcquireSouvenir.IsEmpty())
			{
				GameInstance->AcquireSouvenir=LoadedGame->AcquireSouvenir;
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

int32 UGameSaveController::FindLatestSaveGame()
{
	UTestSaveGame* LatestSave = nullptr;
	int32 LatestSlotNum=-1;
	FDateTime LatestTime = FDateTime(0); // 가장 과거의 시간으로 초기화

	int32 MaxSlots=4;
	
	for (int32 SlotIndex = 0; SlotIndex < MaxSlots; ++SlotIndex)
	{
		FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
		
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			UTestSaveGame* LoadedSave = Cast<UTestSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
			if (LoadedSave)
			{
				// 가장 최근 데이터인지 비교
				if (LoadedSave->DateTime > LatestTime)
				{
					LatestTime = LoadedSave->DateTime;
					LatestSave = LoadedSave;
				}
			}
		}
	}
	
	if (LatestSave)
	{
		return LatestSlotNum= LatestSave->SlotNum;
	}
	
	return LatestSlotNum;
}
