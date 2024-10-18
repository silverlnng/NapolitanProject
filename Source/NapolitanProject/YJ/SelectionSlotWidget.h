// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionSlotWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectionSlotBtn,int, result);

UCLASS()
class NAPOLITANPROJECT_API USelectionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	class ATestPlayerController* TestPlayerController;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Selection;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Selection;

	UPROPERTY()
	int32 result=0;

	FSelectionSlotBtn SelectionSlotBtnDele;
	
	UFUNCTION()
	void OnClickedSelection();
	UFUNCTION()
	void SetTextSelection(FString str);
	
};
