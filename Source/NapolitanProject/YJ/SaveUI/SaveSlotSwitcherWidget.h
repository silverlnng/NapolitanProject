// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlotSwitcherWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USaveSlotSwitcherWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SlotSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UVacantSaveWidget* WBP_VacantSlot;

	UPROPERTY(meta = (BindWidget))
	class USavedSlotWidget* WBP_SavedSlot;
	
	UPROPERTY(VisibleAnywhere)
	int32 SlotNumber;
	
	UFUNCTION()
	void SetActiveIndex(int index);
};
