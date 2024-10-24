// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogueWidget.h"

#include "SelectionSlotWidget.h"
#include "TestPlayerController.h"
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

void UNPCDialogueWidget::SetText_Dialogue(const FString& str)
{
	//Text_Dialogue->SetColorAndOpacity(FLinearColor(1,1,1,1));
	//Text_Dialogue->SetFont()
	Text_Dialogue->SetText(FText::FromString(str));
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
	
	for (int i=0;i<count;i++)
	{
		USelectionSlotWidget* newSlot=CreateWidget<USelectionSlotWidget>(this,SelectionSlotUIFactory);
		newSlot->SetTextSelection(*str[i]);
		newSlot->result=result[i];
		UniformGridPanel_Selection->AddChildToUniformGrid(newSlot,i,0);
	}
}
