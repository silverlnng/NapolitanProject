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
	class UNPCInfoWidget* WBP_NPCInfo;
	UPROPERTY(meta = (BindWidget))
	class UUserWidget* WBP_SettingInNote;
	UPROPERTY(meta = (BindWidget))
	class UUserWidget* WBP_RuleInfo;

	UPROPERTY(meta = (BindWidget))
	class UClueInfoWidget* WBP_ClueInfo;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Rule;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Souvenir;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Setting;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ClueInfo;

///////// NPC Info //////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Docent;
	UFUNCTION()
	void OnClickBtn_Btn_Docent();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Security;
	UFUNCTION()
	void OnClickBtn_Btn_Security();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Cleaner;

	UFUNCTION()
	void OnClickBtn_Btn_Cleaner();
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_Content;

	UFUNCTION()
	void SelectContent(int32 idx);

	UFUNCTION()
	void OnClickBtn_RuleInfo();

	UFUNCTION()
	void OnClickBtn_Souvenir();

	UFUNCTION()
	void OnClickBtn_Setting();

	UFUNCTION()
	void OnClickBtn_ClueInfo();

//////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* ClickSoundWave;
	
	
};

