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

	////// 피격당했을때 나올 animation ////////////
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	class UWidgetAnimation* HitAnim;

	UFUNCTION()
	void PlayHitAnim();
	
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
	//UPROPERTY(meta=(BindWidget))
	
	UPROPERTY(meta=(BindWidget))
	class UBorder* Border_Note;
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	class UWidgetAnimation* NoteImgAnim;
	UFUNCTION()
	void PlayNoteUIEvent(bool val);

/////////// 단서 ui ///////////
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CanvasPanel_Clue;
	UPROPERTY(meta=(BindWidget))
	class URichTextBlock* RichTextBlock_Clue;

	UPROPERTY(meta=(BindWidget))
	class UImage* Img_ClueContent;
	
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* CanvasPanel_Hit;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_ClueClose;

	void SetImgClueContent(UTexture2D* Texture);
	
	void SetRichText_Clue(FString str);

	void SetVisibleCanvasPanel_Clue(bool val);

	UFUNCTION()
	void OnClickBtn_ClueClose();
};
