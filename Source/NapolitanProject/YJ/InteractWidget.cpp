// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HBox_GetSouvenir->SetVisibility(ESlateVisibility::Hidden);
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
