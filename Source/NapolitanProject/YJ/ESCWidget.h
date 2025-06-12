// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UESCWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_StartLevel;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Continue;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> StartLevel;
	
	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	UPROPERTY(VisibleAnywhere)
	class USaveGISubsystem* SaveGI;
	
	UFUNCTION()
	void OnClick_Exit();
	
	UFUNCTION()
	void OnClick_StartLevel();

	UFUNCTION()
	void OnClick_Continue();
	
};
