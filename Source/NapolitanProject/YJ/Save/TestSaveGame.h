// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "TestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UTestSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	//레벨 정보
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString PlayerLevel;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector PlayerLocation;
	// 플레이어 회전
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FRotator PlayerRotation;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString SaveTime;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FDateTime DateTime;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int SlotNum;

	UPROPERTY(VisibleAnywhere, Category = "SaveData") // 저장을 한 장소  
	FString SaveLocation;
	

	// npc id 와 state를 저장
	UPROPERTY(BlueprintReadWrite)
	TMap<int32,int32> NPCStates;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<int32> ClearedNPC;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<FName> NPCEventManage;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<FString> QuestSlots;

	UPROPERTY()
	TSet<int32> AcquireSouvenir;

	UPROPERTY()
	TArray<TSubclassOf<class AItemActor>> SavedItems;
	
	// 그동안 모은 단서
	UPROPERTY()
	TMap<int32,FClueData> AcquireClueMap;

	UPROPERTY()
	TArray<TSoftObjectPtr<UWorld>> SubLevelArray;

	// 세팅값 저장하기
	UPROPERTY(VisibleAnywhere)
	float TotalSoundMix=1.f;
	UPROPERTY(VisibleAnywhere)
	float BGSoundMix=1.f;
	UPROPERTY(VisibleAnywhere)
	float SFXSoundMix=1.f;
};
