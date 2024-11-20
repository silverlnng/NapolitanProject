// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInfoWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

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
	},1.0f,false);
	

	FTimerHandle UITimer2;

	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this, wsNum]()
	{
		if (wsNum==1)
		{
			WidgetSwitcher_Cleaner1->SetActiveWidgetIndex(1);
		}
		else if (wsNum==2)
		{
			WidgetSwitcher_Cleaner2->SetActiveWidgetIndex(1);
		}
		
	},2.0f,false);
	
}
