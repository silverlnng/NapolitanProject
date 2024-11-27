// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionSlotWidget.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USelectionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TestPlayerController=GetOwningPlayer<ATestPlayerController>();
	Btn_Selection->OnClicked.AddDynamic(this,&USelectionSlotWidget::OnClickedSelection);
	Btn_Selection->OnHovered.AddDynamic(this,&USelectionSlotWidget::OnHoverSelection);
}

void USelectionSlotWidget::OnClickedSelection()
{
	// 버튼에 할당된 값과 지금 대화하는 npc의 값을 비교하기
	// 지금 대화하는 npc는 주인공 캐릭터를 통해 알수있
	//SelectionSlotBtnDele.Broadcast(result);
	TestPlayerController->CallCurNPCResultEvent(result);

	if (NPCEventSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, NPCEventSoundWave);
	}
}

void USelectionSlotWidget::OnHoverSelection()
{
	if (HoverAnim) 
	{
		//HoverAnim 되는도안 계속 실행됨
		PlayAnimation(HoverAnim,0,1);
	}
}

void USelectionSlotWidget::SetTextSelection(FString str)
{
	Text_Selection->SetText(FText::FromString(str));
}
