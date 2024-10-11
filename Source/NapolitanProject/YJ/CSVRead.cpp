// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVRead.h"

#include "Serialization/Csv/CsvParser.h"


// Sets default values
ACSVRead::ACSVRead()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSVRead::BeginPlay()
{
	Super::BeginPlay();

	LoadDialogueFromCSV(FPaths::ProjectDir() / TEXT("npc_Dialogue.csv"));
	FString Dialogue = GetNPCDialogue(TEXT("1"), TEXT("2"),0,TEXT("kor"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialogue);

	
	LoadResultFromCSV(FPaths::ProjectDir() / TEXT("npc_result2.csv"));
	FString result = GetNPCResult(TEXT("1"), TEXT("2"), TEXT("Bad"),TEXT("kor"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
}

// Called every frame
void ACSVRead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool ACSVRead::LoadDialogueFromCSV(const FString& FilePath)
{
	FString CSVContent; // csv 내용을 저장할 변수
    
	// CSV 파일 읽기
	if (!FFileHelper::LoadFileToString(CSVContent, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *FilePath);
		return false;
	}
	// 먼저 행 단위로 나누기 
	FCsvParser Parser(CSVContent);
	const FCsvParser::FRows& Rows = Parser.GetRows();

	
	// 첫 번째 줄 (헤더) 건너뛰기 
	for (int32 RowIdx = 1; RowIdx < Rows.Num(); ++RowIdx)
	{
		const TArray<const TCHAR*>& Row = Rows[RowIdx]; // 1행 부터 ....n 행
		
		
		// 1행의 0 ,1 ,2 ,3 ....열
		
		int32 npc_id=FCString::Atoi(Row[0]);
		int32 npc_state=FCString::Atoi(Row[1]);
		int32 npc_order=FCString::Atoi(Row[2]);
		int32 FindKey =(npc_id*100)+(npc_state*10)+npc_order;
		
		FNPCDialogue Dialogue;
		Dialogue.Dialogue_Kor = Row[4];
		Dialogue.Dialogue_Eng = Row[5];
		
		
		// NPC 대사를 맵에 저장
		NPCDialogueMap.Add(FindKey, Dialogue);
	}
	return true;
}

bool ACSVRead::LoadResultFromCSV(const FString& FilePath)
{
	// FString CSVPath = FPaths::ProjectDir() / TEXT("NPCData.csv");
	FString CSVContent; // csv 내용을 저장할 변수
    
	// CSV 파일 읽기
	if (!FFileHelper::LoadFileToString(CSVContent, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *FilePath);
		return false;
	}
	// 먼저 행 단위로 나누기 
	FCsvParser Parser(CSVContent);
	const FCsvParser::FRows& Rows = Parser.GetRows();

	
	// 첫 번째 줄 (헤더) 건너뛰기 
	for (int32 RowIdx = 1; RowIdx < Rows.Num(); ++RowIdx)
	{
		const TArray<const TCHAR*>& Row = Rows[RowIdx]; // 1행 부터 ....n 행
		
		
		// 1행의 0 ,1 ,2 ,3 ....열
		
		int32 npc_id=FCString::Atoi(Row[0]);
		int32 npc_state=FCString::Atoi(Row[1]);
		int32 FindKey =(npc_id*100)+npc_state;
		
		FNPCResult Dialogue;
		Dialogue.result_Good_Kor = Row[3];
		Dialogue.result_bad_Kor = Row[4];
		Dialogue.result_Good_Eng= Row[5];
		Dialogue.result_bad_Eng = Row[6];
		
		// NPC 대사를 맵에 저장
		NPCResultMap.Add(FindKey, Dialogue);
	}
	return true;
}

FString ACSVRead::GetNPCResult(const FString& NPC_ID, const FString& State, const FString& Result,const FString& Lang)
{
	int32 npc_id=FCString::Atoi(*NPC_ID);
	int32 npc_state=FCString::Atoi(*State);
	int32 FindKey =(npc_id*100)+npc_state;
	
	if (!NPCResultMap.Contains(FindKey))
	{
		return TEXT("NPC를 찾을 수 없습니다.");
	}

	const FNPCResult& Dialogue = NPCResultMap[FindKey];

	if (Result == TEXT("Good"))
	{
		return (Lang == TEXT("kor")) ? Dialogue.result_Good_Kor : Dialogue.result_Good_Eng;
	}
	else if (Result == TEXT("Bad"))
	{
		return (Lang == TEXT("kor")) ? Dialogue.result_bad_Kor : Dialogue.result_bad_Eng;
	}
	return TEXT("대사를 찾을 수 없습니다.");
}

FString ACSVRead::GetNPCDialogue(const FString& NPC_ID, const FString& State, const int32 order, const FString& Lang )
{
	// 매개변수로 NPC_ID , State 가 주어지면 order 에 따라서 dialogue출력될수있도록 만들기
	int32 npc_id=FCString::Atoi(*NPC_ID);
	int32 npc_state=FCString::Atoi(*State);
	int32 FindKey =(npc_id*100)+npc_state*10+order;
	
	if (!NPCResultMap.Contains(FindKey))
	{
		return TEXT("NPC를 찾을 수 없습니다.");
	}

	const FNPCDialogue& Dialogue = NPCDialogueMap[FindKey];

	if (Lang == TEXT("kor"))
	{
		return Dialogue.Dialogue_Kor;
	}
	else
	{
		return Dialogue.Dialogue_Eng;
	}
	
	return TEXT("대사를 찾을 수 없습니다.");
}
