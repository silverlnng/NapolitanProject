// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UInvenSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Thumnail;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_InvenSlot;

	UPROPERTY()
	int32 ItemIndex;
	
	
	UFUNCTION()
	void OnClickedInvenSlot();
	UFUNCTION()
	void OnHoverInvenSlot();
};
