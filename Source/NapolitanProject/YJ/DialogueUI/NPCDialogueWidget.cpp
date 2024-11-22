// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogueWidget.h"

#include "SelectionSlotWidget.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "NapolitanProject/NapolitanProject.h"


void UNPCDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TestPC=GetOwningPlayer<ATestPlayerController>();
	Btn_Back->OnClicked.AddDynamic(this,&UNPCDialogueWidget::OnClickbackButton);
	Btn_Next->OnClicked.AddDynamic(this,&UNPCDialogueWidget::OnClickfrontButton);
	Btn_Exit->OnClicked.AddDynamic(this,&UNPCDialogueWidget::OnClickExitButton);
}



void UNPCDialogueWidget::OnClickbackButton()
{
	if(curOrder<0){return;}
	curOrder-=1;
	TestPC->SetNPCDialougueText(curOrder);
}

void UNPCDialogueWidget::OnClickfrontButton()
{
	// 다음 ui 가 출력되도록
	if(curOrder>MaxOrder){return;}
	curOrder+=1;
	UE_LOG(LogTemp,Warning,TEXT("%s,curOrder : %d"),*CALLINFO,curOrder);
	TestPC->SetNPCDialougueText(curOrder);
}

void UNPCDialogueWidget::OnClickExitButton()
{
	TestPC->StartEndNPCDialougue(false);
}

void UNPCDialogueWidget::SetText_Dialogue(const FString& str)
{
	CurrentText="";
	FullText=str;
	
	// 앞쪽 태그 찾아내기 >
	int32 StartTagEnd = FullText.Find(TEXT(">"))+1;
	// 앞쪽 태그 잘라낸 문장
	RemoveTags=FullText.Mid(StartTagEnd,FullText.Len());
	// 태그
	StartTag = FullText.Mid(0,StartTagEnd);
	
	GetWorld()->GetTimerManager().SetTimer(TextUpdateTimerHandle, this, &UNPCDialogueWidget::UpdateText, TextUpdateInterval, true);
	
	//Text_Dialogue->SetText(FText::FromString(str));
}

void UNPCDialogueWidget::UpdateText()
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
	if (Text_Dialogue)
	{
		Text_Dialogue->SetText(FText::FromString(StartTag+CurrentText+TEXT("</>")));
	}
}


void UNPCDialogueWidget::SetSelectSlotVisible(bool value)
{
	if (value)
	{
		UniformGridPanel_Selection->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UniformGridPanel_Selection->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNPCDialogueWidget::CreateSelectionChildren(int32 count,TArray<FString> str,TArray<int32> result)
{
	// 여기서 반복문 돌려서 생성시키기,,,
	// 모든 차일드 지우고 생성시키기
	for (UWidget* wid : UniformGridPanel_Selection->GetAllChildren())
	{
		UniformGridPanel_Selection->RemoveChild(wid);
	}
	// 반복문으로 갯수만큼 생성시키기
		// 값이 없으면 생성을 안하게 됨
	
	for (int i=0;i<count;i++)
	{
		USelectionSlotWidget* newSlot=CreateWidget<USelectionSlotWidget>(this,SelectionSlotUIFactory);
		newSlot->SetTextSelection(*str[i]);
		newSlot->result=result[i];
		UniformGridPanel_Selection->AddChildToUniformGrid(newSlot,i,0);
	}
}

void UNPCDialogueWidget::UIEffect(FString& str)
{
	// str 값에 따라서 이벤트 작동시키기
	if (str=="Red")
	{
		
	}
}
