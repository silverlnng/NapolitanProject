// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadEndingWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "Save/GameSaveController.h"

void UDeadEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UDeadEndingWidget::OnRestart);
	QuitButton->OnClicked.AddDynamic(this, &UDeadEndingWidget::OnQuit);

	TestPlayerController=GetOwningPlayer<ATestPlayerController>();
	MainCharacter =TestPlayerController->GetPawn<ATestCharacter>();
	
	GI=GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	
	IMG_BG->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_description->SetText(FText::FromString(TEXT("")));
	
}

void UDeadEndingWidget::SetTextBlock_description(const FString& Str)
{
	IMG_BG->SetVisibility(ESlateVisibility::Visible);
	CurrentText=FString(TEXT(""));
	FullText=Str;
	
	GetWorld()->GetTimerManager().SetTimer(TextUpdateTimerHandle, this, &UDeadEndingWidget::UpdateText, TextUpdateInterval, true);
}

void UDeadEndingWidget::UpdateText()
{
	// 전체 텍스트의 끝까지 도달하면 타이머 중지
	if (CurrentText.Len() >= FullText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextUpdateTimerHandle);
		// 끝나는 시점 에 특정함수를 실행할수 있도록 만들기
		
		return;
	}
	
	// 한 글자씩 추가
	CurrentText += FullText.Mid(CurrentText.Len(), 1);
	
	// TextBlock에 적용
	if (TextBlock_description)
	{
		TextBlock_description->SetText(FText::FromString(CurrentText));
	}
}


void UDeadEndingWidget::OnRestart()
{
	//재시작 버튼을 누르면 현재 레벨을 다시 시작하고 싶다.
	
	MainCharacter->bIsBeingAttacked=false;
	
	MainCharacter->StopSound();
		
	MainCharacter->SetPlayerState(EPlayerState::Idle);
	this->SetVisibility(ESlateVisibility::Hidden);

	// 가장 최근에 저장한것을 로드하기
	if (!GameSaveController)
	{
		UE_LOG(LogTemp , Warning , TEXT("%s : GameSaveController is nullptr!"),*CALLINFO);
	}


	// IsFromLoad true 이면 불러온것을 로드함
	if (SaveGI->IsFromLoad)
	{
		if (SaveGI->LoadedGame)
		{
			int SlotNum=SaveGI->LoadedGame->SlotNum;
			SaveGI->GameSaveController->LoadGameFromSlot(SlotNum);
		}
	}
	else // 레벨 이동,로드플레이가 아닌경우, 로드 플레이중 한번이라도 저장한게 있는경우.  
	{
		if (GameSaveController&&GameSaveController->FindLatestSaveGame()!=-1)
		{
			int32 SlotNum=GameSaveController->FindLatestSaveGame(); //최근껄 로드 
			GameSaveController->LoadGameFromSlot(SlotNum);
			UE_LOG(LogTemp , Warning , TEXT("%s : FindLatestSaveGame is: %d!"),*CALLINFO,SlotNum);
	
		}
		else
		{
			// 없으면 지하 처음자리에서 시작하기
			// 그냥 로비레벨 열기
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,LobbyLevel,true); //레벨 변경
		
		}
		// 저장한것 없으면 처음 시작 .
	}
	
	
	
}

void UDeadEndingWidget::OnQuit()
{
	SaveGI->SetLevelMoveToDoor(false);
	//종료 버튼 누르면 게임 종료
	auto* pc = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false);
}
