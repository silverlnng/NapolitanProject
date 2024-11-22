// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SouvenirWidgetFix.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USouvenirWidgetFix : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Left;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Right;
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* HorizontalBox_;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_0;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_1;
	
	UFUNCTION()
	void OnclickedAdjustCanvasPanelRatio();

	UFUNCTION()
	void OnclickedAdjustCanvasPanelRatioOrigin();

};
