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
	
	IMG_BG->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_description->SetText(FText::FromString(TEXT("")));
	RichText_Name->SetVisibility(ESlateVisibility::Hidden);
}

void UDeadEndingWidget::SetRichText_Name(const FString& Str) const
{
	RichText_Name->SetText(FText::FromString(Str));
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

void UDeadEndingWidget::StartLerpTimer()
{
	// 페이드 애니 실행시키기
	if (FadeAnim)
	{
		// 애니메이션 재생
		PlayAnimation(FadeAnim);
	}
	
}

void UDeadEndingWidget::UpdateLerp()
{
	/*ElapsedTime += 0.01f; // 타이머 호출 간격만큼 시간 증가

	// Lerp 비율 계산
	float Alpha = FMath::Clamp(ElapsedTime / LerpDuration, 0.0f, 1.0f);
	
	
	float Alp = FMath::Lerp(Img_Fade->GetColorAndOpacity().A, 1, Alpha);
	
	Img_Fade->SetColorAndOpacity(FLinearColor(0,0,0,Alp));

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
	}*/
}

void UDeadEndingWidget::OnRestart()
{
	//재시작 버튼을 누르면 현재 레벨을 다시 시작하고 싶다.
	//FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	//UGameplayStatics::OpenLevel(GetWorld(), FName(*mapName));

	// 베타에서는 그냥 처음 start자리로 가도록 하기
	//MainCharacter->TeleportTo(FVector(-1410,-2020,-656),FRotator(0,-90,0));
	//MainCharacter->SetActorLocation(FVector(-1410,2020,-656),false,);
	//MainCharacter->SetActorRotation(FRotator(0,-90,0));
	MainCharacter->bIsBeingAttacked=false;
	
	MainCharacter->StopSound();
		
	MainCharacter->SetPlayerState(EPlayerState::Idle);
	this->SetVisibility(ESlateVisibility::Hidden);

	// 가장 최근에 저장한것을 로드하기
	if (!GameSaveController)
	{
		UE_LOG(LogTemp , Warning , TEXT("%s : GameSaveController is nullptr!"),*CALLINFO);
	}
	
	if (GameSaveController&&GameSaveController->FindLatestSaveGame()!=-1)
	{
		int32 SlotNum=GameSaveController->FindLatestSaveGame();
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

void UDeadEndingWidget::OnQuit()
{
	GI->SetLevelMoveToDoor(false);
	//종료 버튼 누르면 게임 종료
	auto* pc = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false);
}
