// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_CrossHair;
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* HBox_Interact;

	void SetVisibleCrossHair(bool value);
	void SetVisibleHBox(bool value);

	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* HBox_GetSouvenir;
	
};
