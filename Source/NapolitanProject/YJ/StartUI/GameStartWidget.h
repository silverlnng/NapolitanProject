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
	class UButton* Btn_Exit;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Achievement;

	UPROPERTY(meta = (BindWidget))
	class USettingWidget* SettingUI;
	
	UPROPERTY(meta = (BindWidget))
	class ULoadScreenWidget* WBP_LoadScreen;
	
	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	UPROPERTY()
	class USaveGISubsystem* SaveGI;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> MainLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* HoverSoundWave;
	
	UFUNCTION()
	void OnClickedNewGame();
	UFUNCTION()
	void OnClickedLoad();
	UFUNCTION()
	void OnClickedSetting();
	UFUNCTION()
	void OnClick_Exit();
	UFUNCTION()
	void OnClick_Achievement();

	UFUNCTION()
	void OnHoverPlaySound();
};
