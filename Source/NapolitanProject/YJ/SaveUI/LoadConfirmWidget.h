// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"
#include "LoadConfirmWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotClickProtection,bool,val);

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

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ReSave;	
	
	UFUNCTION()
	void OnClickYes();
	
	UFUNCTION()
	void OnClickNo();

	UFUNCTION()
	void OnClickReSave();
	
	UPROPERTY(VisibleAnywhere)
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget;

	UPROPERTY(EditDefaultsOnly) 
	FString SaveLocation=FString(TEXT("2층 전시관 앞"));

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	UPROPERTY(VisibleAnywhere)
	class USaveGISubsystem* SaveGI;
	UFUNCTION()
	void HandleVisibilityChanged(ESlateVisibility InVisibility);
	
	UPROPERTY()
	FSlotClickProtection SlotClickProtection;
private:
	UPROPERTY()
	UGameSaveController* SaveGameManager;
};
