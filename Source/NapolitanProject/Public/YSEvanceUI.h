// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YSEvanceUI.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UYSEvanceUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	//텍스트 애니메이션
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetAnim), Transient);
	class UWidgetAnimation* TextAnim;

	UFUNCTION()
	void PlayAnim();

};
