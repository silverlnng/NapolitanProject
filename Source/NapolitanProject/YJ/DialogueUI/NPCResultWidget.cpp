// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCResultWidget.h"

#include "NPCDialogueWidget.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

void UNPCResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TestPC=GetOwningPlayer<ATestPlayerController>();
	Btn_Back->OnClicked.AddDynamic(this,&UNPCResultWidget::OnClickbackButton);
	Btn_Next->OnClicked.AddDynamic(this,&UNPCResultWidget::OnClickfrontButton);
	Btn_Exit->OnClicked.AddDynamic(this,&UNPCResultWidget::OnClickExitButton);
}

void UNPCResultWidget::SetText_Result(const FString& str)
{
	CurrentText="";
	FullText=str;
	
	// 앞쪽 태그 찾아내기 >
	int32 StartTagEnd = FullText.Find(TEXT(">"))+1;
	// 앞쪽 태그 잘라낸 문장
	RemoveTags=FullText.Mid(StartTagEnd,FullText.Len());
	// 태그
	StartTag = FullText.Mid(0,StartTagEnd);
	
	GetWorld()->GetTimerManager().SetTimer(TextUpdateTimerHandle, this, &UNPCResultWidget::UpdateText, TextUpdateInterval, true);
}

void UNPCResultWidget::UpdateText()
{
	// 전체 텍스트의 끝까지 도달하면 타이머 중지
	if (CurrentText.Len() >= RemoveTags.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TextUpdateTimerHandle);
		return;
	}
	
	// 한 글자씩 추가
	
	CurrentText += RemoveTags.Mid(CurrentText.Len(), 1);

	//CurrentText.Append("</>");
	
	// TextBlock에 적용
	if (Text_Result)
	{
		Text_Result->SetText(FText::FromString(StartTag+CurrentText+TEXT("</>")));
	}
}

void UNPCResultWidget::OnClickbackButton()
{
	if(curOrder<0){return;}
	curOrder-=1;
	TestPC->SetNPCResultText(curOrder);
}

void UNPCResultWidget::OnClickfrontButton()
{
	if(curOrder>MaxOrder){return;}
	curOrder+=1;
	UE_LOG(LogTemp,Warning,TEXT("%s,curOrder : %d"),*CALLINFO,curOrder);
	TestPC->SetNPCResultText(curOrder);
}

void UNPCResultWidget::OnClickExitButton()
{
	//
	TestPC->EndResult();
}

