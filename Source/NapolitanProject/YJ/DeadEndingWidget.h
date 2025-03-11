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
	virtual void NativeConstruct() override;
	
public:

	class ATestPlayerController* TestPlayerController;
	class ATestCharacter* MainCharacter;

	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY()
	class UGameSaveController* GameSaveController;
	
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichText_Name;

	void SetRichText_Name(const FString& Str) const;

	/*UPROPERTY(meta=(BindWidget))
	class UImage* Img_Fade;*/
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeAnim;

	FTimerHandle LerpTimerHandle;
	float ElapsedTime = 0.0f;
	float ElapsedTime2 = 0.0f;
	float LerpDuration = 12.0f;
	void StartLerpTimer();
	void UpdateLerp();

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RestartButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void OnRestart();
	UFUNCTION()
	void OnQuit();

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyLevel;
	
};
