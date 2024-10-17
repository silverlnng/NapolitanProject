// Fill out your copyright notice in the Description page of Project Settings.


#include "VacantSaveWidget.h"

#include "SaveConfirmWidget.h"
#include "Components/Button.h"

void UVacantSaveWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_SaveGame->OnClicked.AddDynamic(this,&UVacantSaveWidget::OnClicked_SaveGame_Clicked);
}

void UVacantSaveWidget::OnClicked_SaveGame_Clicked()
{
	// 여기서 확인창 생성 ??
	// 위젯생성하고
	SaveConfirmUI =CreateWidget<USaveConfirmWidget>(this,SaveConfirmWidgetFactory);

	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveConfirmUI->myParentNum=SlotNumber;
	
	SaveConfirmUI->AddToViewport(1);
	
}
