// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClueSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "ClueInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UClueInfoWidget : public UUserWidget
{
	GENERATED_BODY()

// 데이터 테이블 읽어서  갯수만큼 생성하기
public:

	void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* ScrollBox_List;

	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichText_ClueName;

	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichText_ClueInfo;

	void SetClueContent(int32 Clue_ID);

	// clue 슬롯을 미리 배열로 가지고 있기
	TMap<int,UClueSlotWidget*> ClueSlots;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UClueSlotWidget> ClueSlotWidgetFactory;
};
