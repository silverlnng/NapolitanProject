// Fill out your copyright notice in the Description page of Project Settings.


#include "YSEvanceUI.h"
#include "Components/TextBlock.h"

void UYSEvanceUI::NativeConstruct()
{
	Super::NativeConstruct();

	//초기화
	CurrentText = "";
	CurrentIndex =0;

	//타이머 시작
	GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UYSEvanceUI::UpdateText, TypingSpeed, true);
}

void UYSEvanceUI::UpdateText()
{
	// 타겟 텍스트 길이에 도달하면 타이머 종료
	if (CurrentIndex >= TargetText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		return;
	}

	// 한 글자 추가
	CurrentText.AppendChar(TargetText[CurrentIndex]);
	CurrentIndex++;

	// TextBlock 업데이트
	if (EvanceText)
	{
		EvanceText->SetText(FText::FromString(CurrentText));
	}
}
