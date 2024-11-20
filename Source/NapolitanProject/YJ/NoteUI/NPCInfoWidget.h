// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPCInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_;

/////////////////// 청소부 ///////////////////////////
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Cleaner;

	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* Anim_Cleaner_1;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Cleaner1;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Cleaner2;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Cleaner1;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Cleaner2;
	
////////////////////////////////////////////////////////////	
	
	UFUNCTION()
	void SelectContent(int32 idx);

	UFUNCTION(BlueprintCallable)
	void SetForcus_ScrollBox_Cleaner(int32 panelNum,int32 wsNum);
	
};
