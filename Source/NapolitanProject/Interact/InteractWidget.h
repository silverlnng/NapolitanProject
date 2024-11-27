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
	class ATestPlayerController* pc;
	class ATestCharacter* MainCharacter;
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

////////// 수첩 ui ////////
	//UPROPERTY(meta=(BindWidget))
	
	UPROPERTY(meta=(BindWidget))
	class UBorder* Border_Note;
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	class UWidgetAnimation* NoteImgAnim;
	UFUNCTION()
	void PlayNoteUIEvent(bool val);
	
//////////// 퀘스트 ui //////////////////

	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* VBox_Quest; 

	// 생성한 (받은) 퀘스트를 배열로 가지고 있기
	TArray<class UQuestSlotWidget*> QuestSlotsArray;

	// 1. 미술관을 탐색하자 : 도슨트에게 수첩을 얻고 난 뒤의 퀘스트
	// 2. 머리를 찾아보자 : 
	// 3. 머리를 가져다 주자 : 경비원에게 머리를 얻었을 때 퀘스트
	// 4. 숨겨진 공간을 찾아보자 : 청소부의 유품과 아이템 즉 열쇠를 얻었을 때의 퀘스트
	// 5. 탈출구를 찾아라 : 점프스케어 후 열쇠 획득 시 나오는 퀘스트


	
	// 퀘스트 슬롯 생성, 제거
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuestSlotWidget> QuestSlotWidgetFactory;

	void AddQuestSlot(int32 QuestNum,FString& str);

	void RemoveQuestSlot(int32 RemoveQuestNum);
	
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

///////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* ClueSoundWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* PickUpSoundWave;
	
};
