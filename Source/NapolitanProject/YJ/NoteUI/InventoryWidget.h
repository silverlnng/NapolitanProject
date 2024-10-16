// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// 게임 저장 ,로드와 상관없이 데이터 테이블 아이템에 대한 정보를 불러와서 초기화 하기

	
	
};
