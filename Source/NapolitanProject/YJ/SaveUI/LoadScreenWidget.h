// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	// 지금현재 클릭한 vacant slot이 뭔지 알고 있어야 함
	// 저장 묻는 창 나오도록 하기
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel;
	
	UPROPERTY(meta=(BindWidget))
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget_0;

	UPROPERTY(meta=(BindWidget))
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget_1;

	UPROPERTY(meta=(BindWidget))
	class USaveSlotSwitcherWidget* SaveSlotSwitcherWidget_2;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USaveConfirmWidget> SaveQuestWidgetFactory;
	
	UPROPERTY(meta=(BindWidget))
	class USaveConfirmWidget* SaveConfirmWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, class USaveSlotSwitcherWidget*> SaveSlotSwitcherList;
	
	UFUNCTION()
	void OnSaveQuestionWidgetCreate_0();
	UFUNCTION()
	void OnSaveQuestionWidgetCreate_1();
	UFUNCTION()
	void OnSaveQuestionWidgetCreate_2();
	
};
