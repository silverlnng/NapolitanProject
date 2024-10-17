// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SavedSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USavedSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 SlotNumber;
};
