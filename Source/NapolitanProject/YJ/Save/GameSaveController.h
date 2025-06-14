// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestSaveGame.h"
#include "UObject/NoExportTypes.h"
#include "GameSaveController.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UGameSaveController : public UObject
{
	GENERATED_BODY()
public:	
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGameToSlot(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	UTestSaveGame* LoadGameFromSlot(int32 SlotIndex);

	// 슬롯 이름 생성
	FString GetSlotName(int32 SlotIndex) const;

	// 게임시작시 로드하기 - 게임인스턴스에서 실행시킬 것 
	TArray<UTestSaveGame*> LoadAllSlotInfo(int32 MaxSlots);

	// 가장 최근에 저장한 게임로드를 가져오는 방법
	int32 FindLatestSaveGame();
	int32 UUIDCounter = 0;
private:
	const FString SlotPrefix = TEXT("SaveSlot_");
};


