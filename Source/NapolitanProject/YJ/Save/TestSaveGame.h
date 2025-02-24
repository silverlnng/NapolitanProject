// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
	
	// 아이템 획득,단서 획득 , npc 들간의 관계성 ,클리어 여부
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TMap<FName,bool> ClueStates;

	// npc id 와 state를 저장
	UPROPERTY(BlueprintReadWrite)
	TMap<int32,int32> NPCStates;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<int32> ClearedNPC;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<FName> NPCEventManage;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TSet<FString> QuestSlots;
};
