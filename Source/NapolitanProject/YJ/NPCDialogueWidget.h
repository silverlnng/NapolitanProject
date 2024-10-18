// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCDialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPCDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget)) 
	class UButton* Btn_Back;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Next;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Dialogue;

	UFUNCTION()
	void OnClickbackButton();
	UFUNCTION()
	void OnClickfrontButton();
	UFUNCTION()
	void SetText_Dialogue(const FString& str);

	/// 동적으로 UniformGridPanel_Selection 의 자식으로 선택지를 생성시켜야 함 
	UPROPERTY(meta=(BindWidget))
	class UUniformGridPanel* UniformGridPanel_Selection;

	UFUNCTION()
	void CreateSelectionChildren();
	
};
