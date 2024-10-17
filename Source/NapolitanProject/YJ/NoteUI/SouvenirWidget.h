// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SouvenirWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSouvenirBtn_Back,int, cur);

UCLASS()
class NAPOLITANPROJECT_API USouvenirWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Souvenir_left;
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Souvenir_right;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Souvenir_left;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_Souvenir_right;

	// AItemActor의 TMap<FString,FItemData> SouvenirMap 읽어와서 버튼으로 차례 대로 읽기
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Souvenir_Back;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Souvenir_front;

	UPROPERTY(VisibleAnywhere)
	int32 curPage=0; // 현재 페이지 중 작은 숫자 . 0 ,2 ,4 ... 으로 생각하기 

	UPROPERTY(EditAnywhere,Blueprintable)
	int32 MaxPage=4;

	FSouvenirBtn_Back SouvenirBtn_BackDele;
	
	UFUNCTION()
	void OnClicked_Souvenir_Back();
	UFUNCTION()
	void OnClicked_Souvenir_front();
	
};
