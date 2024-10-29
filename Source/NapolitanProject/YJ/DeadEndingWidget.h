// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UDeadEndingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichText_Name;

	void SetRichText_Name(const FString& Str) const;

	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Fade;

	FTimerHandle LerpTimerHandle;
	float ElapsedTime = 0.0f;
	float LerpDuration = 12.0f;
	void StartLerpTimer();
	void UpdateLerp();
};
