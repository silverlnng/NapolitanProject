// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSaveController.h"

#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UGameSaveController::SaveGameToSlot(int32 SlotIndex)
{
	ATestPlayerController* PlayerController =Cast<ATestPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController) return;

	ATestCharacter* PlayerCharacter = Cast<ATestCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;

	UMyTestGameInstance* GameInstance = Cast<UMyTestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	USaveGISubsystem* SaveGISubsystem = GameInstance->GetSubsystem<USaveGISubsystem>();
 

	
	FString SlotName = GetSlotName(SlotIndex);
	UTestSaveGame* SaveGameInstance = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(UTestSaveGame::StaticClass()));
	
	if (!SaveGameInstance){return;}

	SaveGameInstance->SlotNum = SlotIndex;
	// 데이터 저장 (예: 플레이어 위치)
	//SaveGameInstance->PlayerLocation = PlayerCharacter->GetActorLocation(); 
	//SaveGameInstance->PlayerRotation =PlayerCharacter->GetActorRotation();

	SaveGameInstance->PlayerLocation = PlayerCharacter->SaveTransform.GetLocation();
	SaveGameInstance->PlayerRotation = PlayerCharacter->SaveTransform.GetRotation().Rotator();
	SaveGameInstance->PlayerLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	// 저장한 날짜 
	FDateTime Now = FDateTime::Now();
	SaveGameInstance->SaveTime = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
	SaveGameInstance->DateTime = Now;

	// 저장한 위치
	SaveGameInstance->SaveLocation = PlayerCharacter->SaveLocationStr;

	//
	for (auto subLevel : SaveGISubsystem->SubLevelArray)
	{
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("SaveGISubsystem_Array")));
	}

	SaveGameInstance->SubLevelArray = SaveGISubsystem->SubLevelArray;
	//
	for (auto subLevel : SaveGameInstance->SubLevelArray)
	{
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("SaveGameInstance_Array")));
	}

	// 획득한 단서에 대해 저장하기

	SaveGameInstance->AcquireClueMap = GameInstance->ClueMap;

	// 클리어한 npc 정보를 저장하기
	if (!SaveGISubsystem->ClearedNPC.IsEmpty())
	{
		SaveGameInstance->ClearedNPC = SaveGISubsystem->ClearedNPC;
	}

	/*if (!SaveGISubsystem->NPCEventManage.IsEmpty())
	{
		SaveGameInstance->NPCEventManage = SaveGISubsystem->NPCEventManage;
	}*/
	

	if (!SaveGISubsystem->QuestSlots.IsEmpty())
	{
		SaveGameInstance->QuestSlots = SaveGISubsystem->QuestSlots;
	}

	if (!SaveGISubsystem->AcquireSouvenir.IsEmpty())
	{
		SaveGameInstance->AcquireSouvenir = SaveGISubsystem->AcquireSouvenir;
	}

	if (!SaveGISubsystem->SavedItems.IsEmpty())
	{
		SaveGameInstance->SavedItems = SaveGISubsystem->SavedItems;
	}

	SaveGameInstance->QuestCommandsMap=GameInstance->QuestCommandsMap;
	
	SaveGameInstance->SaveditemDataMap=GameInstance->itemDataMap;
	
	SaveGameInstance->CatchSpiderNum=SaveGISubsystem->CatchSpiderNum;
	UE_LOG(LogTemp,Warning,TEXT("%s SaveGI CatchSpider :%s"),*CALLINFO,*SaveGISubsystem->CatchSpiderNum);

	
	
	SaveGISubsystem->LoadedGame=SaveGameInstance; // 새롭게 저장한걸로 LoadedGame 을 바꾸기
	//SaveGISubsystem->IsFromLoad = false;
	
	// 설정값을 저장하기
	SaveGameInstance->TotalSoundMix = SaveGISubsystem->TotalSoundMix;
	SaveGameInstance->BGSoundMix = SaveGISubsystem->BGSoundMix;
	SaveGameInstance->SFXSoundMix = SaveGISubsystem->SFXSoundMix;

	// 슬롯에 저장
	UGameplayStatics::SaveGameToSlot(SaveGameInstance , SlotName , 0);

	UE_LOG(LogTemp , Warning , TEXT("Game saved to slot: %s") , *SlotName);
	UE_LOG(LogTemp , Warning , TEXT("Game saved to slot: %s") , *UGameplayStatics::GetCurrentLevelName(GetWorld()));
	
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
		
		auto LoadedGameCopy = LoadedGame;
		
		
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, LoadedGameCopy]()
		{
			for (auto subLevel:LoadedGameCopy->SubLevelArray)
			{
				FLatentActionInfo LatentAction;
				LatentAction.CallbackTarget = this;
				LatentAction.UUID = UUIDCounter++;
				LatentAction.Linkage = 0;
				LatentAction.ExecutionFunction = NAME_None;
				UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),subLevel,true,true,LatentAction);

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LoadedGame_SubLevelArray")));
			}
			
		}, 1.0f, false);
		
		
		
		UMyTestGameInstance* GameInstance = Cast<UMyTestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		USaveGISubsystem* SaveGISubsystem = GameInstance->GetSubsystem<USaveGISubsystem>();
		
		if (GameInstance&&SaveGISubsystem)
		{
			// 로드해야 하는 서브레벨
			SaveGISubsystem->SubLevelArray=LoadedGame->SubLevelArray;

			// 단서
			if (!LoadedGame->AcquireClueMap.IsEmpty())
			{
				GameInstance->ClueMap = LoadedGame->AcquireClueMap;
			}
		

			//// 클리어한 npc 정보를 로드해서 전달하기
			if (!LoadedGame->ClearedNPC.IsEmpty())
			{

				for (int32 &val:LoadedGame->ClearedNPC)
				{
					UE_LOG(LogTemp, Warning, TEXT("LoadedGame->ClearedNPC %s,%d"),*CALLINFO,val);
				}
				
				SaveGISubsystem->ClearedNPC=LoadedGame->ClearedNPC;
				for (int32 &val:SaveGISubsystem->ClearedNPC)
				{
					UE_LOG(LogTemp, Warning, TEXT("GameInstance->ClearedNPC %s,%d"),*CALLINFO,val);
					//제대로 되는지 로그로 확인하기

					//그리고 클리어한 npc는 레벨에 나오지 않도록 작업해야함 . 
				}
			}

			
			if (!LoadedGame->QuestSlots.IsEmpty())
			{
				SaveGISubsystem->QuestSlots=LoadedGame->QuestSlots;
			}

			if (!LoadedGame->AcquireSouvenir.IsEmpty())
			{
				SaveGISubsystem->AcquireSouvenir=LoadedGame->AcquireSouvenir;
			}

			if (!LoadedGame->SavedItems.IsEmpty())
			{
				SaveGISubsystem->SavedItems=LoadedGame->SavedItems;
			}

			GameInstance->QuestCommandsMap=LoadedGame->QuestCommandsMap;
			
			GameInstance->itemDataMap=LoadedGame->SaveditemDataMap;
			
			SaveGISubsystem->CatchSpiderNum=LoadedGame->CatchSpiderNum;
			
			UE_LOG(LogTemp,Warning,TEXT("%sLoadedGame CatchSpider :%s"),*CALLINFO,*LoadedGame->CatchSpiderNum);
			UE_LOG(LogTemp,Warning,TEXT("%sSaveGI CatchSpider :%s"),*CALLINFO,*SaveGISubsystem->CatchSpiderNum);
		}
		// 설정값을 로드하기 
		SaveGISubsystem->TotalSoundMix=LoadedGame->TotalSoundMix;
		SaveGISubsystem->BGSoundMix=LoadedGame->BGSoundMix;
		SaveGISubsystem->SFXSoundMix=LoadedGame->SFXSoundMix;
		

		SaveGISubsystem->IsFromLoad=true;
		
		SaveGISubsystem->LoadedGame=LoadedGame; // 불러온게 뭔지 저장을 해두기 
		
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
