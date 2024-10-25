// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameInstance.h"
#include "NapolitanProject/NapolitanProject.h"
#include "Serialization/Csv/CsvParser.h"

void UTestGameInstance::Init()
{
	Super::Init();
	DT_itemData = LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Item.DT_Item'"));
	itemDataRowNames = DT_itemData->GetRowNames();
	
	DT_SouvenirData=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/Item/DT_Souvenir.DT_Souvenir'"));
	SouvenirDataRowNames=DT_SouvenirData->GetRowNames();

	// 언어선택 기본값 한글 :0 ,영어:1
	lang=0;

	// 로드해서 저장해두기 
	LoadDialogueFromCSV(FPaths::ProjectDir() / TEXT("NPC_Dialogue.csv"));
	LoadResultFromCSV(FPaths::ProjectDir() / TEXT("NPC_Result.csv"));
	LoadSelectFromCSV(FPaths::ProjectDir() / TEXT("NPC_Selection.csv"));
		
}

void UTestGameInstance::SetGameInstanceLang(int32 value)
{
	lang=value;
}

int32 UTestGameInstance::GetGameInstanceLang()
{
	return lang;
}

bool UTestGameInstance::LoadResultFromCSV(const FString& FilePath)
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
		const TArray<const TCHAR*>& col = Rows[RowIdx]; // 1행 부터 ....n 행
		
		
		// 1행의 0 ,1 ,2 ,3 ....열
		
		int32 FindKey=FCString::Atoi(col[2]);
		
		FNPCResult Dialogue;
		Dialogue.result_Good_Kor = col[3];
		Dialogue.result_bad_Kor = col[4];
		Dialogue.result_Good_Eng= col[5];
		Dialogue.result_bad_Eng = col[6];
		
		// NPC 대사를 맵에 저장
		NPCResultMap.Add(FindKey, Dialogue);
	}
	return true;
}

bool UTestGameInstance::LoadDialogueFromCSV(const FString& FilePath)
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
		const TArray<const TCHAR*>& Col = Rows[RowIdx]; // 1행 부터 ....n 행
		
		
		// 1행의 0 ,1 ,2 ,3 ....열
		
		int32 FindKey=FCString::Atoi(Col[3]);
			
		FNPCDialogue Dialogue;
		Dialogue.Who = Col[4];
		Dialogue.Dialogue_Kor = Col[5];
		Dialogue.Dialogue_Eng = Col[6];
		Dialogue.CameraEffect = Col[7];
		UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*Dialogue.Dialogue_Kor);
		// NPC 대사를 맵에 저장
		NPCDialogueMap.Add(FindKey, Dialogue);
	}
	return true;
}

bool UTestGameInstance::LoadSelectFromCSV(const FString& FilePath)
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
		const TArray<const TCHAR*>& Col = Rows[RowIdx]; // 1행 부터 ....n 행
		
		
		// 1행의 0 ,1 ,2 ,3 ....열
		
		
		int32 FindKey=FCString::Atoi(Col[3]);
			
		FNPCSelect NPCSelect;
		NPCSelect.Check=FCString::Atoi(Col[4]);
		NPCSelect.Select_Kor = Col[5];
		NPCSelect.Select_Eng = Col[5];
		
		
		// NPC Select 를  NPCSelectMap 맵에 저장
		NPCSelectMap.Add(FindKey, NPCSelect);
	}
	return true;
}

void UTestGameInstance::GetNPCSelect(const int32& NPC_ID, const int32& State, const FString& Lang)
{
	int32 npc_id=NPC_ID;
	int32 npc_state=State;
	
	int32 FindKey =(npc_id*100)+(npc_state*10); // 시작하는 키값

	// 110~115 까지 키값을 순회하기 => 선택지는 최대 5개까지니까 

	for (int i=FindKey ; i<=(FindKey+5); i++)
	{
		if (NPCSelectMap.Contains(FindKey)) // 있는 경우
		{
			// 위젯생성 하고 값 넣어주기 
			
			const FNPCSelect& Select = NPCSelectMap[FindKey];
			if (Lang == TEXT("kor"))
			{
				// Select.Select_Kor;
			}
			else if(Lang == TEXT("eng"))
			{
				// Select.Select_Eng;
			}
		}
		else // 없는경우
		{
			UE_LOG(LogTemp,Warning,TEXT("Select 를 찾을 수 없습니다."));
			return;
		}
	}
}
