// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeRuleWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEscapeRuleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Img_Souvenir1->SetVisibility(ESlateVisibility::Hidden);
	Img_Souvenir2->SetVisibility(ESlateVisibility::Hidden);
	Img_Souvenir3->SetVisibility(ESlateVisibility::Hidden);
	
	TextBlock_Souvenir_1->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_Souvenir_2->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_Souvenir_3->SetVisibility(ESlateVisibility::Hidden);
	
	Img_Key->SetOpacity(0.f);
}

void UEscapeRuleWidget::SetAcquireImage(int32 num,UTexture2D* thumnail)
{
	// num을 받아서 이미지 채우기
	switch (num)
	{
	case 1:
		Img_Souvenir1->SetVisibility(ESlateVisibility::Visible);
		Img_Souvenir1->SetBrushFromTexture(thumnail);
		TextBlock_Souvenir_1->SetVisibility(ESlateVisibility::Visible);
		break;
	case 2:
		Img_Souvenir2->SetVisibility(ESlateVisibility::Visible);
		Img_Souvenir2->SetBrushFromTexture(thumnail);
		TextBlock_Souvenir_2->SetVisibility(ESlateVisibility::Visible);
		break;
	case 3:
		Img_Souvenir3->SetVisibility(ESlateVisibility::Visible);
		Img_Souvenir3->SetBrushFromTexture(thumnail);
		TextBlock_Souvenir_3->SetVisibility(ESlateVisibility::Visible);
		break;
		
	}
}

void UEscapeRuleWidget::SetKeyImage(int32 num)
{
	switch (num)
	{
	case 1:
		Img_Key->SetOpacity(0.35f);
		break;
	case 2:
		Img_Key->SetOpacity(0.5f);
		break;
	case 3:
		Img_Key->SetOpacity(1.f);
		break;
	}
}
