// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UGameStartWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_NewGame;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Load;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Setting;

	UPROPERTY(meta = (BindWidget))
	class USettingWidget* SettingUI;

	UFUNCTION()
	void OnClickedNewGame();
	UFUNCTION()
	void OnClickedLoad();
	UFUNCTION()
	void OnClickedSetting();
	
};
