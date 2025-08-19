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
	class USaveGISubsystem* SaveGI;

	UPROPERTY()
	class UGameSaveController* GameSaveController;

////////////////////////////// 새롭게 사망 글 적기//////////////////	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextBlock_description;

	void SetTextBlock_description(const FString& Str);

	void SetTextBlock_description_(const FString& Str,UTextBlock* Text_description);

	FString CurrentText;
	FString FullText;
	UPROPERTY(EditAnywhere)
	float  TextUpdateInterval = 0.1f;
	FTimerHandle TextUpdateTimerHandle;

	//// 한글자씩 업데이트 효과
	void UpdateText();

	void UpdateText_(UTextBlock* Text_description);
	
////////////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RestartButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void OnRestart();
	UFUNCTION()
	void OnQuit();

	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_BG;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyLevel;


	UFUNCTION()
	void HandleVisibilityChanged(ESlateVisibility InVisibility);
};

