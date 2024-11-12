// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionSlotWidget.h"
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
	class URichTextBlock* Text_Dialogue;

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

	UFUNCTION()
	void OnClickbackButton();
	UFUNCTION()
	void OnClickfrontButton();
	UFUNCTION()
	void OnClickExitButton();
	
	UFUNCTION()
	void SetText_Dialogue(const FString& str);

	/// 동적으로 UniformGridPanel_Selection 의 자식으로 선택지를 생성시켜야 함
	UPROPERTY()
	TMap<int,USelectionSlotWidget*> SelectionSlotMap;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USelectionSlotWidget> SelectionSlotUIFactory;
	
	
	UPROPERTY(meta=(BindWidget))
	class UUniformGridPanel* UniformGridPanel_Selection;

	UFUNCTION()
	void SetSelectSlotVisible(bool value);
	
	UFUNCTION()
	void CreateSelectionChildren(int32 count,TArray<FString> str,TArray<int32> result);

	UFUNCTION()
	void UIEffect(FString &str);
	
};
