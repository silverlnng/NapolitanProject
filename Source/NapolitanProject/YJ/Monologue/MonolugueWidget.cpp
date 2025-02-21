// Fill out your copyright notice in the Description page of Project Settings.


#include "MonolugueWidget.h"

#include "Components/RichTextBlock.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"


void UMonolugueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TestPC=GetOwningPlayer<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	
	OnVisibilityChanged.AddDynamic(this,&UMonolugueWidget::HandleVisibilityChanged);
	
}

void UMonolugueWidget::HandleVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		if (MainCharacter)
		{
			MainCharacter->PlaySound(MonologueSound);
		}
	}
	else if (InVisibility == ESlateVisibility::Hidden)
	{
		if (MainCharacter)
		{
			MainCharacter->StopSound();
		}
	}
}

void UMonolugueWidget::SetText_Dialogue(const FString& str)
{
	CurrentText="";
	FullText=str;
	
	// 앞쪽 태그 찾아내기 >
	int32 StartTagEnd = FullText.Find(TEXT(">"))+1;
	// 앞쪽 태그 잘라낸 문장
	RemoveTags=FullText.Mid(StartTagEnd,FullText.Len());
	// 태그
	StartTag = FullText.Mid(0,StartTagEnd);
	
	GetWorld()->GetTimerManager().SetTimer(TextUpdateTimerHandle, this, &UMonolugueWidget::UpdateText, TextUpdateInterval, true);
}

void UMonolugueWidget::UpdateText()
{
	// 전체 텍스트의 끝까지 도달하면 타이머 중지
	if (CurrentText.Len() >= RemoveTags.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextUpdateTimerHandle);
		// 끝나는 시점 에 특정함수를 실행할수 있도록 만들기

		// 다음줄로 넘어가기 
		if (CurrentIndex < OutputLines.Num()-1)
		{
			
			// 인덱스 증가
			CurrentIndex++;
		
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				SetText_Dialogue(OutputLines[CurrentIndex]);
				MainCharacter->PlaySound(MonologueSound);
			}, 1.f, false);
		}
		else // 맨마지막줄 출력
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				SetVisibility(ESlateVisibility::Hidden);
				// 남아있는 text 지우기
				Text_Monologue->SetText(FText::FromString(("")));
			}, 1.f, false);
			return;
		}
		
	}
	
	// 한 글자씩 추가
	
	CurrentText += RemoveTags.Mid(CurrentText.Len(), 1);

	//CurrentText.Append("</>");
	
	// TextBlock에 적용
	if (Text_Monologue)
	{
		Text_Monologue->SetText(FText::FromString(StartTag+CurrentText+TEXT("</>")));
	}
}

void UMonolugueWidget::SetOutputLines(const TArray<FString>& InputTextLines)
{
	OutputLines.Empty(); // 기존 데이터를 비웁니다.
	CurrentIndex=0;
	for (const FString& Line : InputTextLines)
	{
		OutputLines.Add(Line);
	}

	// OutputLines 을 받은 다음 한줄씩 출력 시작

	if (OutputLines.Num() > 0)
	{
		SetText_Dialogue(OutputLines[0]);
	}
}

