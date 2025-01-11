// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"
#include "LoadConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ULoadConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	 
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Yes;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_No;	
	
	UFUNCTION()
	void OnClickYes();
	
	UFUNCTION()
	void OnClickNo();
	
	UPROPERTY(VisibleAnywhere)
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget;
private:
	UPROPERTY()
	UGameSaveController* SaveGameManager;
};
