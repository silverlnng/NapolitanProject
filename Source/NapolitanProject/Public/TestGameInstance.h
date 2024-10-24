// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TestGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool IsHad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 ItemInt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UStaticMesh* ItemMesh;
};

USTRUCT(BlueprintType)
struct FSouvenirData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	bool IsHad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 SouvenirInt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString SouvenirInfo;
};

USTRUCT(BlueprintType)
struct FNPCResult
{
	GENERATED_BODY()
	FString State;
	FString result_Good_Kor;
	FString result_bad_Kor;
	FString result_Good_Eng;
	FString result_bad_Eng;
};

USTRUCT(BlueprintType)
struct FNPCDialogue
{
	GENERATED_BODY()
	FString Who;
	FString Font;
	FString Color;
	FString Dialogue_Kor;
	FString Dialogue_Eng;
};

USTRUCT(BlueprintType)
struct FNPCSelect
{
	GENERATED_BODY()
	int32 Check;
	FString Select_Kor;
	FString Select_Eng;
};


UCLASS()
class NAPOLITANPROJECT_API UTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

//////////// 소모성 아이템 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_itemData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> itemDataRowNames;
	
	//////////// 유물 데이터 ///////////////////////	
	UPROPERTY(VisibleAnywhere)
	UDataTable* DT_SouvenirData;
	UPROPERTY(VisibleAnywhere)
	TArray<FName> SouvenirDataRowNames;

	//////////// 언어 선택 ///////////////////////
	UPROPERTY(VisibleAnywhere)
	int32 lang;

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
	FString GetNPCResult(const FString& NPC_ID, const FString& State, const FString& Result ,const FString& Lang);
	
	FString GetNPCDialogue(const FString& NPC_ID, const FString& State, const int32 order ,const FString& Lang);

	void GetNPCSelect(const int32& NPC_ID,const int32& State,const FString& Lang);
	// npc 만날떄 한번 호출로 선택지 모두 생성시키기 
	
	
};

