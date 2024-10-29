// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadEndingWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDeadEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UDeadEndingWidget::OnRestart);
	QuitButton->OnClicked.AddDynamic(this, &UDeadEndingWidget::OnQuit);
}

void UDeadEndingWidget::SetRichText_Name(const FString& Str) const
{
	RichText_Name->SetText(FText::FromString(Str));
}

void UDeadEndingWidget::StartLerpTimer()
{
	ElapsedTime = 0.0f;
	
	GetWorld()->GetTimerManager().SetTimer(LerpTimerHandle,this,&UDeadEndingWidget::UpdateLerp,0.01f, true);
}

void UDeadEndingWidget::UpdateLerp()
{
	ElapsedTime += 0.01f; // 타이머 호출 간격만큼 시간 증가

	// Lerp 비율 계산
	float Alpha = FMath::Clamp(ElapsedTime / LerpDuration, 0.0f, 1.0f);
	
	
	float Alp = FMath::Lerp(Img_Fade->GetColorAndOpacity().A, 1, Alpha);
	
	Img_Fade->SetColorAndOpacity(FLinearColor(0,0,0,Alp));

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
	}
}

void UDeadEndingWidget::OnRestart()
{
	//재시작 버튼을 누르면 현재 레벨을 다시 시작하고 싶다.
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	UGameplayStatics::OpenLevel(GetWorld(), FName(*mapName));
}

void UDeadEndingWidget::OnQuit()
{
	//종료 버튼 누르면 게임 종료
	auto* pc = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false);
}
