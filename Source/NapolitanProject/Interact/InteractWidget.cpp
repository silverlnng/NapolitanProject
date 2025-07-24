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
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/QuestSlotWidget.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HBox_GetSouvenir->SetVisibility(ESlateVisibility::Hidden);
	Border_Note->SetVisibility(ESlateVisibility::Hidden);
	// Border_Inven->SetVisibility(ESlateVisibility::Hidden);
	CanvasPanel_Clue->SetVisibility(ESlateVisibility::Hidden);
	CanvasPanel_PictureLabel->SetVisibility(ESlateVisibility::Hidden);
	
	Btn_ClueClose->OnClicked.AddDynamic(this,&UInteractWidget::OnClickBtn_ClueClose);
	Btn_PictureLabelClose->OnClicked.AddDynamic(this,&UInteractWidget::OnClickBtn_PictureLabelClose);
	
	pc =GetOwningPlayer<ATestPlayerController>();
	MainCharacter=pc->GetPawn<ATestCharacter>();
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


void UInteractWidget::GetSouvenirEvent(const FString& str)
{
	HBox_GetSouvenir->SetVisibility(ESlateVisibility::Visible);
	Text_SouvenirName->SetText(FText::FromString(str));
	PlayAnimation(GetSouvenirAnim);
	if (PickUpSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, PickUpSoundWave);
	}
}

void UInteractWidget::PlayNoteUIEvent(bool val)
{
	if (val)
	{
		Border_Note->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(NoteImgAnim);
	}
	
}

void UInteractWidget::PlayInvenUIEvent()
{
	PlayAnimation(InvenImgAnim);
}

void UInteractWidget::GetItemEvent(const FString& str)
{
	Text_ItemName->SetText(FText::FromString(str));
	PlayAnimation(GetItemAnim);
}

void UInteractWidget::LoadUpdateQuestSlot()
{
	if (SaveGI&&!SaveGI->QuestSlots.IsEmpty())
	{
		for (auto &QuestSlot:SaveGI->QuestSlots)
		{
			
			UQuestSlotWidget* QuestSlotWidget = CreateWidget<UQuestSlotWidget>(this, QuestSlotWidgetFactory);
			// 어떤 퀘스트 인지 ??
			if (QuestSlotWidget)
			{
				// VerticalBox에 추가
				// 만들때 아이디를 부여해서
				QuestSlotWidget->SetText_QuestName(QuestSlot);
				VBox_Quest->AddChildToVerticalBox(QuestSlotWidget);
				QuestSlotsArray.Add(QuestSlot,QuestSlotWidget);
			}
		}
	}
}

void UInteractWidget::AddQuestSlot(int32 QuestNum,FString& str)
{
	//QuestSlotsArray 에서 QuestNum 으로 중복검사하고
	if (QuestSlotsArray.Contains(str)) {return;}

	if (SaveGI)
	{
		SaveGI->QuestSlots.Add(str);
	}
	
	UQuestSlotWidget* QuestSlot = CreateWidget<UQuestSlotWidget>(this, QuestSlotWidgetFactory);
	// 어떤 퀘스트 인지 ??
	if (QuestSlot)
	{
		// VerticalBox에 추가
		// 만들때 아이디를 부여해서
		QuestSlot->SetQuestNum(QuestNum);
		QuestSlot->SetText_QuestName(str);
		VBox_Quest->AddChildToVerticalBox(QuestSlot);
		QuestSlotsArray.Add(str,QuestSlot);
	}
}

void UInteractWidget::RemoveQuestSlot(const FString& str)
{
	//퀘스트를 달성할떄 호출해야함 어떤걸 제거할지 ??

	// 제거할 아이디를 받아서 map에 검색을 하기 

	if (QuestSlotsArray.Contains(str))
	{
		VBox_Quest->RemoveChild(QuestSlotsArray[str]);
		QuestSlotsArray.Remove(str);
		if (SaveGI)
		{
			SaveGI->QuestSlots.Remove(str);
		}
	}
	
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
		if (ClueSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, ClueSoundWave);
		}
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
	
	if (pc)
	{
		if (MainCharacter)
		{
			MainCharacter->SetPlayerState(EPlayerState::Idle);
		}
	}
}

void UInteractWidget::SetText_PictureLabel(FString& Title,FString& descrip)
{
	TextBlock_PictureLabel_Title->SetText(FText::FromString(*Title));
	TextBlock_PictureLabel_Descrip->SetText(FText::FromString(*descrip));
}

void UInteractWidget::SetVisibleCanvasPanel_PictureLabel(bool val)
{
	if (val)
	{
		CanvasPanel_PictureLabel->SetVisibility(ESlateVisibility::Visible);
		if (ClueSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, ClueSoundWave);
		}
	}
	else
	{
		CanvasPanel_PictureLabel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInteractWidget::OnClickBtn_PictureLabelClose()
{
	CanvasPanel_PictureLabel->SetVisibility(ESlateVisibility::Hidden);
	// 캐릭터 모드도 변경
	
	if (pc)
	{
		if (MainCharacter)
		{
			MainCharacter->SetPlayerState(EPlayerState::Idle);
		}
	}
}
