// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/QuestSlotWidget.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HBox_GetSouvenir->SetVisibility(ESlateVisibility::Hidden);
	Border_Note->SetVisibility(ESlateVisibility::Hidden);
	CanvasPanel_Clue->SetVisibility(ESlateVisibility::Hidden);
	CanvasPanel_Hit->SetVisibility(ESlateVisibility::Hidden);
	Btn_ClueClose->OnClicked.AddDynamic(this,&UInteractWidget::OnClickBtn_ClueClose);
}

void UInteractWidget::SetVisibleCrossHair(bool value)
{
	if (value)
	{
		Image_CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteractWidget::SetVisibleHBox(bool value)
{
	if (value)
	{
		HBox_Interact->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HBox_Interact->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteractWidget::PlayHitAnim()
{
	PlayAnimation(HitAnim);
}

void UInteractWidget::GetSouvenirEvent(const FString& str)
{
	HBox_GetSouvenir->SetVisibility(ESlateVisibility::Visible);
	Text_SouvenirName->SetText(FText::FromString(str));
	PlayAnimation(GetSouvenirAnim);
}

void UInteractWidget::PlayNoteUIEvent(bool val)
{
	if (val)
	{
		Border_Note->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(NoteImgAnim);
	}
	
}

void UInteractWidget::AddQuestSlot()
{
	//
	UQuestSlotWidget* QuestSlot = CreateWidget<UQuestSlotWidget>(this, UQuestSlotWidget::StaticClass());
	// 어떤 퀘스트 인지 ??
	if (QuestSlot)
	{
		// VerticalBox에 추가
		VBox_Quest->AddChildToVerticalBox(QuestSlot);
		//QuestSlotsMap.Add(QuestSlot)
	}
}

void UInteractWidget::RemoveQuestSlot()
{
	//퀘스트를 달성할떄 호출해야함 어떤걸 제거할지 ??

	
	
	/*if (VBox_Quest && ItemToRemove)
	{
		// VerticalBox에서 아이템 제거
		VBox_Quest->RemoveChild(ItemToRemove);
	}*/
}


void UInteractWidget::SetImgClueContent(UTexture2D* Texture)
{
	Img_ClueContent->SetVisibility(ESlateVisibility::Visible);
	Img_ClueContent->SetBrushFromTexture(Texture);
}

void UInteractWidget::SetRichText_Clue(FString str)
{
	RichTextBlock_Clue->SetText(FText::FromString(*str));
}

void UInteractWidget::SetVisibleCanvasPanel_Clue(bool val)
{
	if (val)
	{
		CanvasPanel_Clue->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CanvasPanel_Clue->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteractWidget::OnClickBtn_ClueClose()
{
	CanvasPanel_Clue->SetVisibility(ESlateVisibility::Hidden);
	// 캐릭터 모드도 변경
	ATestPlayerController* pc =GetOwningPlayer<ATestPlayerController>();
	if (pc)
	{
		pc->GetPawn<ATestCharacter>()->SetPlayerState(EPlayerState::Idle);
	}
}
