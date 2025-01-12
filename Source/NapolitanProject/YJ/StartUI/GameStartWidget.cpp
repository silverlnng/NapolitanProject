// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartWidget.h"

#include "SettingWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
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
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,MainLevel,true);
	
	// 위치 설정 
}

void UGameStartWidget::OnClickedLoad()
{
	ATestPlayerController* TestPC=GetOwningPlayer<ATestPlayerController>();
	if (TestPC)
	{
		APlayerHUD* PlayerHUD=TestPC->GetHUD<APlayerHUD>();
		if (PlayerHUD)
		{
			PlayerHUD->LoadScreenUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UGameStartWidget::OnClickedSetting()
{
	SettingUI->SetVisibility(ESlateVisibility::Visible);
}
