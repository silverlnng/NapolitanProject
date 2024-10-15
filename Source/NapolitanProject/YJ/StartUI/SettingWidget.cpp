// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Setting->OnClicked.AddDynamic(this,&USettingWidget::OnClickeSetting);
}

void USettingWidget::OnClickeSetting()
{
	// 이시점의 value를 가져와서 세팅하기
	//Slider_Total->GetValue()
}
