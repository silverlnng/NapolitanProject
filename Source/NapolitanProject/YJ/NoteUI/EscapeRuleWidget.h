// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeRuleWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UEscapeRuleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Left;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Right;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Souvenir1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Souvenir2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Souvenir3;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Key;

	UFUNCTION()
	void SetAcquireImage(int32 num,UTexture2D* thumnail);

	UFUNCTION()
	void SetKeyImage(int32 num);
	
};
