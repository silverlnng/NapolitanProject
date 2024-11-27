// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPCInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	
/////////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* StickSoundWave;	
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_;
	
///////////////////// 도슨트 ///////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Docent;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Docent1;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Docent2;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Docent1;
	

/////////////////// 청소부 ///////////////////////////
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Cleaner;

	UPROPERTY(meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* Anim_Cleaner_1;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Cleaner1;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Cleaner2;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Cleaner1;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Cleaner2;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Head;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Head2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Head_BG;
	
//////////////////////경비원//////////////////////////////////////	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Security;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Security1;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Security2;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Security1;

///////////노인///////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Oldman;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Oldman1;
	
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Oldman2;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_Oldman1;


	
/////////////////////////////////////////////////////////////	
	UFUNCTION()
	void SelectContent(int32 idx);

	UFUNCTION(BlueprintCallable)
	void SetForcus_ScrollBox_Cleaner(int32 panelNum,int32 wsNum);

	UFUNCTION(BlueprintCallable)
	void SetForcus_ScrollBox_Security(int32 panelNum,int32 wsNum);

	UFUNCTION(BlueprintCallable)
	void SetForcus_ScrollBox_Oldman(int32 panelNum,int32 wsNum);
	
	UFUNCTION(BlueprintCallable)
	void SetForcus_ScrollBox_Docent(int32 panelNum,int32 wsNum);
	
};
