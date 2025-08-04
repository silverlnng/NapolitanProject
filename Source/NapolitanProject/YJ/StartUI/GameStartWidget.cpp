// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartWidget.h"

#include "SettingWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/YJ/SaveUI/LoadScreenWidget.h"

void UGameStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Btn_NewGame->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedNewGame);
	Btn_Load->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedLoad);
	Btn_Setting->OnClicked.AddDynamic(this,&UGameStartWidget::OnClickedSetting);
	Btn_Exit->OnClicked.AddDynamic(this,&UGameStartWidget::OnClick_Exit);
	Btn_Achievement->OnClicked.AddDynamic(this,&UGameStartWidget::OnClick_Achievement);

	// 호버할때 사운드 넣기 
	Btn_NewGame->OnHovered.AddDynamic(this,&UGameStartWidget::OnHoverPlaySound);
	Btn_Load->OnHovered.AddDynamic(this,&UGameStartWidget::OnHoverPlaySound);
	Btn_Setting->OnHovered.AddDynamic(this,&UGameStartWidget::OnHoverPlaySound);
	Btn_Exit->OnHovered.AddDynamic(this,&UGameStartWidget::OnHoverPlaySound);
	Btn_Achievement->OnHovered.AddDynamic(this,&UGameStartWidget::OnHoverPlaySound);

	
	Btn_Achievement->SetVisibility(ESlateVisibility::Hidden);
	SettingUI->SetVisibility(ESlateVisibility::Hidden);
	
	WBP_LoadScreen->SetVisibility(ESlateVisibility::Hidden);
	
	GI =GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	
	if (SaveGI)
	{
		WBP_LoadScreen->LoadUpdateUI(SaveGI->SaveSlotInfos);
	}
	
}

void UGameStartWidget::OnClickedNewGame()
{
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,MainLevel,true);
	
	// 위치 설정 
}

void UGameStartWidget::OnClickedLoad()
{
	WBP_LoadScreen->SetVisibility(ESlateVisibility::Visible);
}

void UGameStartWidget::OnClickedSetting()
{
	SettingUI->SetVisibility(ESlateVisibility::Visible);
}

void UGameStartWidget::OnClick_Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UGameStartWidget::OnClick_Achievement()
{
	
}

void UGameStartWidget::OnHoverPlaySound()
{
	if (HoverSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, HoverSoundWave);
	}
}
