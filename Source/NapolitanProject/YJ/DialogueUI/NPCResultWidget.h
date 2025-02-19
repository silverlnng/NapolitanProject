// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPCResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget)) 
	class UButton* Btn_Back;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Next;
	
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* Text_Result;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Exit;

	
	FString CurrentText;
	FString FullText;
	FString RemoveTags;
	FString StartTag;
	float  TextUpdateInterval = 0.05f;
	FTimerHandle TextUpdateTimerHandle;

	//// 한글자씩 업데이트 효과
	void UpdateText();
	

	UPROPERTY(VisibleAnywhere)
	int32 curOrder=0;

	UPROPERTY(VisibleAnywhere)
	int32 MaxOrder=0;

	class ATestPlayerController* TestPC;

	UPROPERTY()
	class ASoundControlActor* SoundControlActor;
	
	UFUNCTION()
	void OnClickbackButton();
	UFUNCTION()
	void OnClickfrontButton();
	UFUNCTION()
	void OnClickExitButton();
	
	UFUNCTION()
	void SetText_Result(const FString& str);

	UFUNCTION()
	void HandleVisibilityChanged(ESlateVisibility InVisibility);
};
