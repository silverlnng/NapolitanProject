// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveConfirmWidget.h"
#include "Components/Button.h"

void USaveConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//버튼에
	Btn_Yes->OnClicked.AddDynamic(this,&USaveConfirmWidget::OnClickYes);
}

void USaveConfirmWidget::OnClickYes()
{
	// 나자신을 생성한 위젯에 대한 정보를 알수있나 ??
	
}

void USaveConfirmWidget::OnClickNo()
{
}

