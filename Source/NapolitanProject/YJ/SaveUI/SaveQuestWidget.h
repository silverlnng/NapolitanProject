// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USaveQuestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	 
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Yes;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_No;

	UPROPERTY(VisibleAnywhere)
	int32 myParentNum;

	UPROPERTY(VisibleAnywhere)
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget;
	
	UFUNCTION()
	void OnClickYes();
	
	UFUNCTION()
	void OnClickNo();
};
