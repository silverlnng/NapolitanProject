// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VacantSaveWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UVacantSaveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	// 빈슬롯을 누르면 저장할건 지 말건지 묻는 질문 나오고
	// 실제로 저장 묻는 창 => 실제로 저장해야함 
		// 이게 LoadScreenWidget 에 있는 질문창이 나오도록
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SaveGame;

	UPROPERTY(VisibleAnywhere)
	int32 SlotNumber;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USaveConfirmWidget> SaveConfirmWidgetFactory;
	
	UPROPERTY(meta=(BindWidget))
	class USaveConfirmWidget* SaveConfirmUI;
	
	UFUNCTION()
	void OnClicked_SaveGame_Clicked();
	
};
