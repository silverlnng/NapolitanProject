// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonolugueWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UMonolugueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	class ATestPlayerController* TestPC;
	class ATestCharacter* MainCharacter;
	
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* Text_Monologue;

	UPROPERTY(VisibleAnywhere)
	TArray<FString> OutputLines;
	
	FString CurrentText;
	FString FullText;
	FString RemoveTags;
	FString StartTag;
	UPROPERTY(EditAnywhere)
	float  TextUpdateInterval = 0.1f;
	FTimerHandle TextUpdateTimerHandle;

	// 현재 출력 중인 인덱스
	int32 CurrentIndex;
public:	
	UFUNCTION()
	void SetText_Dialogue(const FString& str);
	
	void UpdateText();
	
public:
	UFUNCTION()
	void SetOutputLines(const TArray<FString>& InputTextLines);

	UFUNCTION()
	void HandleVisibilityChanged(ESlateVisibility InVisibility);

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* MonologueSound;
	
};
