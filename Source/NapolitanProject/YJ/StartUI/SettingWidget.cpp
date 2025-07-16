// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GI = GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	
	Slider_Total->OnValueChanged.AddDynamic(this,&USettingWidget::OnSlider_Total_Changed);
	
	Slider_BG->OnValueChanged.AddDynamic(this,&USettingWidget::OnSlider_BG_Changed);

	Slider_SFX->OnValueChanged.AddDynamic(this,&USettingWidget::OnSlider_SFX_Changed);
	
	Button_Setting->OnClicked.AddDynamic(this,&USettingWidget::OnClickeSetting);
	Button_Exit->OnClicked.AddDynamic(this,&USettingWidget::OnClickeExit);
}

void USettingWidget::OnSlider_Total_Changed(float value)
{
	// 딱한번만 플레이 시키기
	if (SaveGI)
	{
		SaveGI->TotalSoundMix=value;
	}
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),MasterMix,MyMasterClass,value,1,0);
}

void USettingWidget::OnSlider_BG_Changed(float value)
{
	if (SaveGI)
	{
		SaveGI->BGSoundMix=value;
	}
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),BackGroundMix,BackGroundClass,value,1,0);	
}

void USettingWidget::OnSlider_SFX_Changed(float value)
{
	if (SaveGI)
	{
		SaveGI->SFXSoundMix=value;
	}
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),SFXMix,MySFXClass,value,1,0);
	// 테스트 용으로 하나 플레이시키기 
}

void USettingWidget::OnSlider_Setting_Changed(float value)
{
	
}

void USettingWidget::OnClickeSetting()
{
	
}

void USettingWidget::OnClickeExit()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
