// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EventWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UEventWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichText_Event;
};
