// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestGameInstance.h"

#include "Engine/AssetManager.h"
#include "NapolitanProject/NapolitanProject.h"

#include "Serialization/Csv/CsvParser.h"

void UMyTestGameInstance::Init()
{
	Super::Init();

	// 로그 막아두기 
	/*if (GEngine)
	{
		GEngine->bEnableOnScreenDebugMessages = false;
	}*/
	
	DT_itemData = LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/DT/DT_Item.DT_Item'"));
	
	if (DT_itemData)
	{
		UE_LOG(LogTemp, Error, TEXT("%s DT_Item 로드성공"),*CALLINFO);
		itemDataRowNames = DT_itemData->GetRowNames();

		
		for (int i = 0; i < itemDataRowNames.Num(); i++)
		{
			UE_LOG(LogTemp, Error, TEXT("%s,%s"),*CALLINFO,*itemDataRowNames[i].ToString());
			
			FItemData* ItemData = DT_itemData->FindRow<FItemData>(itemDataRowNames[i] , TEXT(""));
			if (ItemData)
			{
				//유물획득 초기화 => 게임로드안했을때 . 게임저장시스템만들면 초기화 안해야함 
				//ItemData->Had=false;
			}
		}
		
	}
	else
	{
		//  LoadObject<UDataTable>을 못한경우 출력할 것
		UE_LOG(LogTemp, Error, TEXT("%s DT_Item 로드하지 못함"),*CALLINFO);
	}
	
	DT_SouvenirData=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/DT/DT_Souvenir.DT_Souvenir'"));

	if (DT_SouvenirData)
	{
		UE_LOG(LogTemp, Error, TEXT("%s DT_Souvenir 로드성공"),*CALLINFO);
		SouvenirDataRowNames=DT_SouvenirData->GetRowNames();

		for (int i = 0; i < SouvenirDataRowNames.Num(); i++)
		{
			FSouvenirData* SouvenirData = DT_SouvenirData->FindRow<FSouvenirData>(SouvenirDataRowNames[i] , TEXT(""));
			if (SouvenirData)
			{
				//유물획득 초기화 => 게임로드안했을때
				//SouvenirData->Had=false;
				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s DT_Souvenir 로드하지 못함"),*CALLINFO);
	}


	// 클루데이터
	// /Script/Engine.DataTable'/Game/YJ/Item/DT_Clue.DT_Clue'

	DT_Clue=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/DT/DT_Clue.DT_Clue'"));

	if (DT_Clue)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s DT_Clue 로드성공"),*CALLINFO);
		ClueDataRowNames=DT_Clue->GetRowNames();

		for (int i = 0; i < ClueDataRowNames.Num(); i++)
		{
			FClueData* ClueData = DT_Clue->FindRow<FClueData>(ClueDataRowNames[i] , TEXT(""));
			if (ClueData)
			{
				//단서 획득 초기화 => 게임로드안했을때
				//ClueData->Had=false;
				
				// 로드 확인용
				//FString cluerowname=ClueDataRowNames[i].ToString();
				//UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*cluerowname);
				//UE_LOG(LogTemp,Warning,TEXT("%s,%d"),*CALLINFO,ClueData->ClueID);
				
			}
		}
	}

	
	DT_Quest=LoadObject<UDataTable>(nullptr ,TEXT("'/Game/YJ/DT/DT_Quest.DT_Quest'"));
	

	// 언어선택 기본값 한글 :0 ,영어:1
	lang=0;

	// 로드해서 저장해두기 
	LoadDialogueFromCSV(FPaths::ProjectContentDir()/ TEXT("CSVFile")/TEXT("NPC_Dialogue.csv"));
	LoadResultFromCSV(FPaths::ProjectContentDir()/ TEXT("CSVFile")/ TEXT("NPC_Result.csv"));
	LoadSelectFromCSV(FPaths::ProjectContentDir()/ TEXT("CSVFile") / TEXT("NPC_Selection.csv"));
	
	// "C:\UnrealProjects\NapolitanProject\NPC_Dialogue.csv"
	// "C:\UnrealProjects\NapolitanProject\NPC_Result.csv"
	// "C:\UnrealProjects\NapolitanProject\NPC_Selection.csv"
	
	// ClearedNPC 는 로드한 정보로 초기화가 되야함 
}

void UMyTestGameInstance::SetGameInstanceLang(int32 value)
{
	lang=value;
}

int32 UMyTestGameInstance::GetGameInstanceLang()
{
	return lang;
}

bool UMyTestGameInstance::LoadResultFromCSV(const FString& FilePath)
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
		
		int32 FindKey=FCString::Atoi(col[4]);
		
		FNPCResult Dialogue;
		Dialogue.result_Kor = col[5];
		Dialogue.result_Eng= col[6];
		Dialogue.EventNameString=col[7];
		UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*Dialogue.EventNameString);
		// NPC 대사를 맵에 저장
		NPCResultMap.Add(FindKey, Dialogue);
	}
	return true;
}

bool UMyTestGameInstance::LoadDialogueFromCSV(const FString& FilePath)
{

	UE_LOG(LogTemp, Error, TEXT("%s Dialogue CSV file: %s"),*CALLINFO,*FilePath);
	
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
		Dialogue.EventNameString = Col[7];
		//UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*Dialogue.Dialogue_Kor);
		UE_LOG(LogTemp,Warning,TEXT("%s,%s"),*CALLINFO,*Dialogue.EventNameString);
		// NPC 대사를 맵에 저장
		NPCDialogueMap.Add(FindKey, Dialogue);
	}
	return true;
}

bool UMyTestGameInstance::LoadSelectFromCSV(const FString& FilePath)
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

FString UMyTestGameInstance::GetNPCResult(const int32& NPC_ID, const int32& State, const int32& SelectedAnswer,
	const FString& Lang)
{
	int32 npc_id=NPC_ID;
	int32 npc_state=State;
	int32 npc_Selected=SelectedAnswer;
	
	int32 FindKey =(npc_id*100)+(npc_state*10)+npc_Selected;
	
	if (NPCResultMap.Contains(FindKey)) // 있는 경우
	{
		// 위젯생성 하고 값 넣어주기 
			
		const FNPCResult& Result = NPCResultMap[FindKey];
		if (Lang == TEXT("kor"))
		{
			// Select.Select_Kor;
			const FString str=Result.result_Kor;
			return Result.result_Kor;
		}
		else if(Lang == TEXT("eng"))
		{
			// Select.Select_Eng;
			const FString str=Result.result_Eng;
			return Result.result_Eng;
		}
	}
	return "FindKey를 찾지 못함";
}

void UMyTestGameInstance::GetNPCSelect(const int32& NPC_ID, const int32& State, const FString& Lang)
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


void UMyTestGameInstance::AsyncLoadLoadLevel(const TSoftObjectPtr<UWorld> Level)
{
	// ✅ TSoftObjectPtr<UWorld> → FSoftObjectPath 변환
	FSoftObjectPath LevelPath = Level->GetPathName();
	// ✅ 비동기 레벨 로딩 시작
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	//Streamable.RequestAsyncLoad(LevelPath);

	// KeepHandle은 GC 방지용
	TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([=]()
	{
		UE_LOG(LogTemp, Log, TEXT("Level Preload Finished!"));
	}));

	// 핸들을 유지하지 않으면 GC가 다시 언로드해버림
	PreloadHandle = Handle; // ex. UPROPERTY로 잡아두기

	if (PreloadHandle.IsValid() && PreloadHandle->HasLoadCompleted())
	{
		UE_LOG(LogTemp, Log, TEXT("Level is already preloaded!"));
	}
}

void UMyTestGameInstance::PreloadLevel(FName& LevelName)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Preload Path: %s"), *LevelName.ToString());
	FSoftObjectPath LevelPath(LevelName);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	PreloadHandle = Streamable.RequestAsyncLoad(LevelPath, FStreamableDelegate::CreateLambda([=]()
	{
		UE_LOG(LogTemp, Log, TEXT("Preload done for %s"), *LevelName.ToString());
	}));
}

void UMyTestGameInstance::UnlockAchievement(FString AchievementId)
{
	/*IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (!Identity.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Identity interface invalid"));
			return;
		}

		TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0); // 로컬 플레이어 인덱스
		if (!UserId.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("User ID invalid"));
			return;
		}

		IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();
		if (Achievements.IsValid())
		{
			// Achievements Write 객체 생성
			FOnlineAchievementsWriteRef WriteObject = MakeShared<FOnlineAchievementsWrite>();
			WriteObject->SetFloatStat(*AchievementId, 100.0f);  // 100% 달성 처리

			// Delegate 바인딩
			FOnAchievementsWrittenDelegate Delegate;
			Delegate.BindLambda([UserId](const FUniqueNetId& PlayerId, bool bSuccess)
			{
				if (bSuccess)
				{
					UE_LOG(LogTemp, Log, TEXT("Achievement successfully written for %s"), *PlayerId.ToString());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Achievement write failed for %s"), *PlayerId.ToString());
				}
			});

			// Achievements Write 호출
			Achievements->WriteAchievements(*UserId, WriteObject, Delegate);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Achievements interface invalid"));
		}
	}*/
}

