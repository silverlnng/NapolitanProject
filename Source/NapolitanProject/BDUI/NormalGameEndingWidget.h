// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormalGameEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNormalGameEndingWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* EndingAnim;

	void PlayEndingAnim();
};
