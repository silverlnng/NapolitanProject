// Fill out your copyright notice in the Description page of Project Settings.


#include "ESCWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "StartUI/SettingWidget.h"

void UESCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Exit->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Exit);
	Btn_StartLevel->OnClicked.AddDynamic(this,&UESCWidget::OnClick_StartLevel);
	Btn_Continue->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Continue);
	Btn_Setting->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Setting);
	
	GI=GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();

	SettingUI->SetVisibility(ESlateVisibility::Hidden);
}

void UESCWidget::OnClick_Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UESCWidget::OnClick_StartLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),StartLevel,true);

	SaveGI->SetLevelMoveToDoor(false); 
}

void UESCWidget::OnClick_Continue()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UESCWidget::OnClick_Setting()
{
	// 세팅창이 보이도록하기
	SettingUI->SetVisibility(ESlateVisibility::Visible);

	// 값세팅해주기
	if (!SaveGI){return;}
	SettingUI->Slider_Total->SetValue(SaveGI->TotalSoundMix);
	SettingUI->Slider_BG->SetValue(SaveGI->BGSoundMix);
	SettingUI->Slider_SFX->SetValue(SaveGI->SFXSoundMix);
}
