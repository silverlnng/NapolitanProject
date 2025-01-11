// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "TestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameModeBase();

	virtual void BeginPlay() override;
	
	// NPC와의 상호작용 후 클리어 상태 업데이트
	void UpdateNPCClearState(const FString& NPCID, bool bIsCleared);

	// 저장과 로드를 위한 메서드
	void AutoSaveGame();
	void LoadLastSave();

private:
	FTimerHandle AutoSaveTimerHandle;
	FString SlotName = TEXT("AutoSaveSlot");  // 저장 슬롯 이름
	uint32 UserIndex = 0;  // 기본 사용자 인덱스

	void SaveGame();
	UTestSaveGame* LoadGame();

	// NPC 상태 초기화 및 복원
	void InitializeNPCStates();
	void RestoreNPCStates(const TMap<int32,int32>& LoadedNPCStates);
};
