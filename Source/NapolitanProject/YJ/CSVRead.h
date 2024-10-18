// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSVRead.generated.h"

/*
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
	FString Dialogue_Kor;
	FString Dialogue_Eng;
};
*/


UCLASS()
class NAPOLITANPROJECT_API ACSVRead : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSVRead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//TMap<int32,FNPCResult> NPCResultMap;
	
	//TMap<int32,FNPCDialogue> NPCDialogueMap;
	
	bool LoadResultFromCSV(const FString& FilePath);
	
	bool LoadDialogueFromCSV(const FString& FilePath);

	//  어떤 npc , 어떤 상황에서 분기점에서 마지막 대사  
	FString GetNPCResult(const FString& NPC_ID, const FString& State, const FString& Result ,const FString& Lang);
	
	FString GetNPCDialogue(const FString& NPC_ID, const FString& State, const int32 order ,const FString& Lang);
};



