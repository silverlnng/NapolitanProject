// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInfoWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UNPCInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Img_Head->SetVisibility(ESlateVisibility::Hidden);
	Img_Head2->SetVisibility(ESlateVisibility::Hidden);
	Img_Head_BG->SetVisibility(ESlateVisibility::Hidden);
}

void UNPCInfoWidget::SelectContent(int32 idx)
{
	WidgetSwitcher_->SetActiveWidgetIndex(idx);
}

void UNPCInfoWidget::SetForcus_ScrollBox_Cleaner(int32 panelNum,int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Cleaner->ScrollWidgetIntoView(CanvasPanel_Cleaner2,true);
		}
	},1.5f,false);
	

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Cleaner1->SetActiveWidgetIndex(1);
			PlayAnimation(Anim_Cleaner_1);
		}
		else if (wsNum==2)
		{
			WidgetSwitcher_Cleaner2->SetActiveWidgetIndex(1);
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
	},2.0f,false);
	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Security(int32 panelNum, int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Security->ScrollWidgetIntoView(CanvasPanel_Security1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Security->ScrollWidgetIntoView(CanvasPanel_Security2,true);
		}
	},1.5f,false);

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Security1->SetActiveWidgetIndex(1);
			
			FTimerHandle UITimer3;
			GetWorld()->GetTimerManager().SetTimer(UITimer3 , [this]()
			{
				Img_Head->SetVisibility(ESlateVisibility::Visible);
				Img_Head2->SetVisibility(ESlateVisibility::Visible);
				Img_Head_BG->SetVisibility(ESlateVisibility::Visible);
			} , 0.5f , false);		
			
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
	},2.5f,false);
	
}

void UNPCInfoWidget::SetForcus_ScrollBox_Oldman(int32 panelNum, int32 wsNum)
{
	FTimerHandle UITimer1;

	GetWorld()->GetTimerManager().SetTimer(UITimer1,[this, panelNum]()
	{
		if (panelNum==1)
		{
			ScrollBox_Oldman->ScrollWidgetIntoView(CanvasPanel_Oldman1,true);
		}
		else if (panelNum==2)
		{
			ScrollBox_Oldman->ScrollWidgetIntoView(CanvasPanel_Oldman2,true);
		}
	},1.5f,false);
	

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Oldman1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			
		}
		if (StickSoundWave)
		{
			UGameplayStatics::PlaySound2D(this, StickSoundWave);
		}
	},2.0f,false);
}
