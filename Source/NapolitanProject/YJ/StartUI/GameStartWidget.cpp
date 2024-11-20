// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartWidget.h"

#include "SettingWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/YJ/SaveUI/LoadScreenWidget.h"

void UGameStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_NewGame->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedNewGame);
	Btn_Load->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedLoad);
	Btn_Setting->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedSetting);

	SettingUI->SetVisibility(ESlateVisibility::Hidden);

	WBP_LoadScreen->SetVisibility(ESlateVisibility::Hidden);
}

void UGameStartWidget::OnClickedNewGame()
{
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),MainLevel,true);
	
	// 위치 설정 
}

void UGameStartWidget::OnClickedLoad()
{
	
}

void UGameStartWidget::OnClickedSetting()
{
	SettingUI->SetVisibility(ESlateVisibility::Visible);
}
