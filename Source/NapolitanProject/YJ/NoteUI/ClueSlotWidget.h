// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClueSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UClueSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere)
	int32 Clue_ID;

	class ATestPlayerController* TestPlayerController;

	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;

	
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_ClueSlot;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_ClueName;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UFUNCTION()
	void OnClickBtn_ClueSlot();
	UFUNCTION()
	void SetWidgetSwitcher(int32 val);
	UFUNCTION()
	void SetTextClueName(FString str);
};
