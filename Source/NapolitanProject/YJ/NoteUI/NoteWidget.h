// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNoteWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* WBP_Inventory;
	UPROPERTY(meta = (BindWidget))
	class USouvenirWidget* WBP_Souvenir;
	UPROPERTY(meta = (BindWidget))
	class UUserWidget* WBP_Rule;
	UPROPERTY(meta = (BindWidget))
	class UUserWidget* WBP_SettingInNote;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Rule;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Souvenir;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Setting;

	UFUNCTION()
	void OnClickBtn_Rule();

	UFUNCTION()
	void OnClickBtn_Souvenir();

	UFUNCTION()
	void OnClickBtn_Setting();
	
};
