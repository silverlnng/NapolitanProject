// Fill out your copyright notice in the Description page of Project Settings.


#include "SouvenirWidgetFix.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void USouvenirWidgetFix::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_0->OnClicked.AddDynamic(this,&USouvenirWidgetFix::OnclickedAdjustCanvasPanelRatio);
	Btn_1->OnClicked.AddDynamic(this,&USouvenirWidgetFix::OnclickedAdjustCanvasPanelRatioOrigin);
}

void USouvenirWidgetFix::OnclickedAdjustCanvasPanelRatio()
{
	UHorizontalBoxSlot* Slot1 = Cast<UHorizontalBoxSlot>(HorizontalBox_->GetChildAt(0)->Slot);
	UHorizontalBoxSlot* Slot2 = Cast<UHorizontalBoxSlot>(HorizontalBox_->GetChildAt(1)->Slot);

	if (!Slot1 || !Slot2)
	{
		UE_LOG(LogTemp, Warning, TEXT("One or both children do not have valid slots."));
		return;
	}
	
	FSlateChildSize Size1;
	Size1.SizeRule = ESlateSizeRule::Fill;
	Size1.Value=0.99f;
	Slot1->SetSize(Size1);

	FSlateChildSize Size2;
	Size2.SizeRule = ESlateSizeRule::Fill;
	Size2.Value=0.01f;
	Slot2->SetSize(Size2);
	
	// Optionally, force a layout update
	HorizontalBox_->InvalidateLayoutAndVolatility();
}

void USouvenirWidgetFix::OnclickedAdjustCanvasPanelRatioOrigin()
{
	UHorizontalBoxSlot* Slot1 = Cast<UHorizontalBoxSlot>(HorizontalBox_->GetChildAt(0)->Slot);
	UHorizontalBoxSlot* Slot2 = Cast<UHorizontalBoxSlot>(HorizontalBox_->GetChildAt(1)->Slot);

	if (!Slot1 || !Slot2)
	{
		UE_LOG(LogTemp, Warning, TEXT("One or both children do not have valid slots."));
		return;
	}
	
	FSlateChildSize Size1;
	Size1.SizeRule = ESlateSizeRule::Fill;
	Size1.Value=1.f;
	Slot1->SetSize(Size1);

	FSlateChildSize Size2;
	Size2.SizeRule = ESlateSizeRule::Fill;
	Size2.Value=1.f;
	Slot2->SetSize(Size2);
	
	// Optionally, force a layout update
	HorizontalBox_->InvalidateLayoutAndVolatility();
}
