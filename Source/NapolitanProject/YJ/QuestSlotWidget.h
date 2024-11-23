// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UQuestSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_QuestName;

	UFUNCTION()
	void SetText_QuestName(FString str);
	
};
