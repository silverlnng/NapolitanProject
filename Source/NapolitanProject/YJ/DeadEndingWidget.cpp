// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadEndingWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"

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
