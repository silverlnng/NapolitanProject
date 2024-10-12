// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogueWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UNPCDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Back->OnClicked.AddDynamic(this,&UNPCDialogueWidget::OnClickbackButton);
	Btn_Next->OnClicked.AddDynamic(this,&UNPCDialogueWidget::OnClickfrontButton);
}

void UNPCDialogueWidget::OnClickbackButton()
{
	
}

void UNPCDialogueWidget::OnClickfrontButton()
{
	// 다음 ui 가 출력되도록
	
}

void UNPCDialogueWidget::SetText_Dialogue(const FString& str)
{
	Text_Dialogue->SetText(FText::FromString(str));
}
