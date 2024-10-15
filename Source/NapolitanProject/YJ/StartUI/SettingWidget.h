// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_Total;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_BG;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_SFX;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Setting;

	UFUNCTION()
	void OnClickeSetting();
};
