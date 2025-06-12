// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTestGameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USaveGISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/////// 플레이하면서 클리어한 NPC , 획득한 유물을 TARRAY 으로 관리하기 /////////////////

	// 월드에 존재하는 npc를 관리 
	UPROPERTY()
	TSet<int32> ClearedNPC;
	UPROPERTY()
	TSet<FName> NPCEventManage; // npc 의 id , 발생한 이벤트 여부

	UPROPERTY()
	TSet<FString> QuestSlots;
	
	UPROPERTY()
	TSet<int32> AcquireSouvenir;

	int32 AcquireSouvenirNum;
	
	//////////////획득한 아이템 관리하기 ///////////////////////	

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class AItemActor>> SavedItems;
	
	//  아이템 저장
	UFUNCTION(BlueprintCallable)
	void SaveAttachedItems();

	//  저장된 아이템 복원
	UFUNCTION(BlueprintCallable)
	void RestoreAttachedItems();

	UPROPERTY()
	FString CatchSpiderNum=TEXT("0");
	///////////// 추리 단서 ////////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_Clue;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> ClueDataRowNames;
	UPROPERTY()
	TMap<int32,FClueData> ClueMap;

	//////////// 게임 저장 /////////////////////

	UPROPERTY()
	class UGameSaveController* GameSaveController;
	UPROPERTY()
	TArray<class UTestSaveGame*> SaveSlotInfos;

	UPROPERTY()
	bool IsFromLoad=false;
	
	UPROPERTY()
	class UTestSaveGame* LoadedGame;
	
	///////// 레벨 이동을  "그림 문 "으로 하는경우를 체크 (문 이동 시 true, 새 게임 시작 시 false)
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bLevelMoveToDoor = false;

	// 플레이어의 이전 위치 저장
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FTransform SavedPlayerTransform;
	
	// 플레이어 위치 저장 함수
	UFUNCTION(BlueprintCallable)
	void SavePlayerFTransform(FTransform NewLocation);

	// 저장된 위치 반환 함수
	UFUNCTION(BlueprintCallable)
	FTransform GetSavedPlayerLocation() const;

	// 위치 사용 여부 플래그 설정 함수
	UFUNCTION(BlueprintCallable)
	void SetLevelMoveToDoor(bool bUse);

	UPROPERTY()
	TSet<TSoftObjectPtr<UWorld>> SubLevelArray;
};
