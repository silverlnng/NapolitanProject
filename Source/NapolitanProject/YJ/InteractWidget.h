// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta=(BindWidget))
	class UImage* Image_CrossHair;
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* HBox_Interact;

	void SetVisibleCrossHair(bool value);
	void SetVisibleHBox(bool value);

	////// 유물 얻었을때 ////////////
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* HBox_GetSouvenir;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_SouvenirName;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	class UWidgetAnimation* GetSouvenirAnim;  // 유물 얻었을때 실행할 이벤트 함수

	UFUNCTION()
	void GetSouvenirEvent(const FString& str); // 유물획득할때 획득한 유물이름 넣어주고 호출하는 함수

	//// 수첩 ui ////////
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Note;
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	class UWidgetAnimation* NoteImgAnim;
	
};
