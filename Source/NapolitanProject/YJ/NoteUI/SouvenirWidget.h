// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SouvenirWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USouvenirWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Souvenir_left;
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Souvenir_right;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Souvenir_left;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Souvenir_right;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Souvenir_Back;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Souvenir_front;
};
