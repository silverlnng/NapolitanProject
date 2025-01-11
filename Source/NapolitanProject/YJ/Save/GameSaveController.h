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

private:
	const FString SlotPrefix = TEXT("SaveSlot_");
};
