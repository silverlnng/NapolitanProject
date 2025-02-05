// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenSlotConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UInvenSlotConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Take;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_No;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Thumnail;
	
	UPROPERTY(VisibleAnywhere)
	class UInvenSlotWidget* CurInvenSlot;
	
	UPROPERTY()
	class AItemActor* MyItem;

	class ATestPlayerController* PC;
	class ATestCharacter* TestCharacter;
		
	UFUNCTION()
	void OnClickTake();
	
	UFUNCTION()
	void OnClickNo();

	UFUNCTION()
	void Set_ImgThumnail(UTexture2D* Texture);
	
};
