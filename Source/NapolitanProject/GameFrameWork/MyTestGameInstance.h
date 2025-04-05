// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetectiveMapGameModeBase.h"
#include "Engine/GameInstance.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "MyTestGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString Name="";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool Had=false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 ItemInt=-1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UStaticMesh* ItemMesh=nullptr;
};

USTRUCT(BlueprintType)
struct FSouvenirData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool Had=false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 SouvenirInt=-1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString SouvenirInfo;
};

USTRUCT(BlueprintType)
struct FNPCResult
{
	GENERATED_BODY()
	FString State;
	FString result_Kor;
	FString result_Eng;
	FString Effect;
};

USTRUCT(BlueprintType)
struct FNPCDialogue
{
	GENERATED_BODY()
	FString Who;
	FString Dialogue_Kor;
	FString Dialogue_Eng;
	FString CameraEffect;
};

USTRUCT(BlueprintType)
struct FNPCSelect
{
	GENERATED_BODY()
	int32 Check;
	FString Select_Kor;
	FString Select_Eng;
};
USTRUCT(BlueprintType)
struct FClueData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString Name="";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool Had=false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 ClueID=-1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString Content="";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* ClueImage=nullptr;
	
};

UCLASS()
class NAPOLITANPROJECT_API UMyTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

//////////// 소모성 아이템 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_itemData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> itemDataRowNames;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,bool> itemDataHadMap;
	
	//////////// 유물 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_SouvenirData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> SouvenirDataRowNames;
	
	//////////// 언어 선택 ///////////////////////
	UPROPERTY(VisibleAnywhere)
	int32 lang=0;

	UFUNCTION()
	void SetGameInstanceLang(int32 value);

	UFUNCTION()
	int32 GetGameInstanceLang();

///////////////////CSV Read 하기
	TMap<int32,FNPCResult> NPCResultMap;
	
	TMap<int32,FNPCDialogue> NPCDialogueMap;

	TMap<int32,FNPCSelect> NPCSelectMap;
	
	bool LoadResultFromCSV(const FString& FilePath);   // NPCResultMap 에 키-대사 넣어주는 함수
	
	bool LoadDialogueFromCSV(const FString& FilePath); // NPCDialogueMap 에 키-대사 넣어주는 함수

	bool LoadSelectFromCSV(const FString& FilePath);
	
	//  어떤 npc , 어떤 상황에서 분기점에서 마지막 대사  
	FString GetNPCResult(const int32& NPC_ID, const int32& State, const int32& SelectedAnswer ,const FString& Lang);
	
	FString GetNPCDialogue(const FString& NPC_ID, const FString& State, const int32 order ,const FString& Lang);

	void GetNPCSelect(const int32& NPC_ID,const int32& State,const FString& Lang);
	// npc 만날떄 한번 호출로 선택지 모두 생성시키기 
	
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

	UPROPERTY()
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

/////// 지금 엔딩 상황 ///////////
	UPROPERTY(VisibleAnywhere)
	EDetectiveMapState DetectiveMapState=EDetectiveMapState::FisrtStart;

//////////// 게임 저장 //////////////
///
	UPROPERTY()
	class UGameSaveController* GameSaveController;
	UPROPERTY()
	TArray<UTestSaveGame*> SaveSlotInfos;


/////////// 레벨전환시 자연스러운 페이드인 아웃 UI 함수 선언
	UFUNCTION(BlueprintCallable)
	void LoadLevelWithLoadingScreen(FName LevelName);
	
	// 로딩 위젯을 저장할 변수
	UPROPERTY()
	UUserWidget* LoadingScreenWidget;
	
///////// 비동기 로비 레벨로드 //////
	UFUNCTION(BlueprintCallable)
	void AsyncLoadLoadLevel(const TSoftObjectPtr<UWorld> Level);
	
	// ✅ 로딩 완료 시 호출되는 함수
	UFUNCTION()
	void OnLevelLoaded();

	UPROPERTY()
	TSoftObjectPtr<UWorld> CachedLevel;

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


	void UnlockAchievement(FString AchievementId);
};

