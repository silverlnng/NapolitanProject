// Fill out your copyright notice in the Description page of Project Settings.


#include "VacantSaveWidget.h"

#include "SaveQuestWidget.h"
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
	SaveQuestWidget =CreateWidget<USaveQuestWidget>(this,SaveQuestWidgetFactory);

	// 클릭한 슬롯 넘버를 parent 넘버로 지정하기 
	SaveQuestWidget->myParentNum=SlotNumber;
	
	SaveQuestWidget->AddToViewport(1);
	
}
