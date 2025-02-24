// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"
#include "SavedSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USavedSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 SlotNumber;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_LoadSelectSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Date;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Loc;
	
	UFUNCTION()
	void SetText_Date(const FString& str);

	UFUNCTION()
	void SetText_Loc(const FString& str);
};
