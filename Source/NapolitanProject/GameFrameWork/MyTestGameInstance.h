// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	/////////CSV Read 하기
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
	
/////// 플레이하면서 클리어한 NPC , 획득한 유물을 TARRAY 으로 관리하기
	TSet<int32> ClearedNPC;
	
	TArray<int32> AcquireSouvenir;

	int32 AcquireSouvenirNum;
	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_Clue;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> ClueDataRowNames;

	TMap<int32,FClueData> ClueMap;

//////////// 게임 저장 //////////////
///
	UPROPERTY()
	class UGameSaveController* GameSaveController;
	UPROPERTY()
	TArray<UTestSaveGame*> SaveSlotInfos;
};

