// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveQuestWidget.h"
#include "Components/Button.h"

void USaveQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//버튼에
	Btn_Yes->OnClicked.AddDynamic(this,&USaveQuestWidget::OnClickYes);
}

void USaveQuestWidget::OnClickYes()
{
	// 나자신을 생성한 위젯에 대한 정보를 알수있나 ??
	
}

void USaveQuestWidget::OnClickNo()
{
}
