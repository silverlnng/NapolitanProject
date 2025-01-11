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
	UE_LOG(LogTemp, Warning, TEXT("OnClicked_SaveGame"));
}
