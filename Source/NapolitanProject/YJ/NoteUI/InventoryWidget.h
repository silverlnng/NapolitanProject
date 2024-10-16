// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvenSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel_Slot;

	// 인벤토리 슬롯을 미리 배열로 가지고 있기
	TMap<int,UInvenSlotWidget*> InvenSlots;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInvenSlotWidget> InvenSlotWidgetFactory;
	
	// create 8번 
	/*UPROPERTY(meta = (BindWidget))
	class UInvenSlotWidget* InvenSlot_0;

	UPROPERTY(meta = (BindWidget))
	class UInvenSlotWidget* InvenSlot_1;
	UPROPERTY(meta = (BindWidget))
	class UInvenSlotWidget* InvenSlot_2;
	UPROPERTY(meta = (BindWidget))
	class UInvenSlotWidget* InvenSlot_3;
	UPROPERTY(meta = (BindWidget))
	class UInvenSlotWidget* InvenSlot_4;*/

	// 게임 저장 ,로드와 상관없이 데이터 테이블 아이템에 대한 정보를 불러와서 초기화 하기

	
	
};
