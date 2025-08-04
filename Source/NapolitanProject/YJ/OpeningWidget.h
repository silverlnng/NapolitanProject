// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningWidget.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UOpeningWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	// 멤버 변수 추가
	FWidgetAnimationDynamicEvent Anim0nFinishedDelegate;
	FWidgetAnimationDynamicEvent Anim1nFinishedDelegate;
	FWidgetAnimationDynamicEvent Anim2nFinishedDelegate;
	FWidgetAnimationDynamicEvent Anim3nFinishedDelegate;
	FWidgetAnimationDynamicEvent Anim4nFinishedDelegate;
	
	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* TicketSequence;

public:
	//class ULevelSequencePlayer* SequencePlayer;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadWrite);
	class UWidgetAnimation* TextAnim_0;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadWrite)
	class UWidgetAnimation* TextAnim_1;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadWrite)
	class UWidgetAnimation* TextAnim_2;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadWrite)
	class UWidgetAnimation* TextAnim_3;

	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadWrite);
	class UWidgetAnimation* TextAnim_4;

	UPROPERTY(meta=(BindWidget))
	class URetainerBox* RetainerBox;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* Media_CanvasPanel;
	
private:
	
	UFUNCTION()
	void OnFirstAnimationFinished();

	UFUNCTION()
	void OnSecondAnimationFinished();

	UFUNCTION()
	void OnThirdAnimationFinished();

	UFUNCTION()
	void OnFourthAnimationFinished();

	UFUNCTION()
	void OnfifthAnimationFinished();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media")
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Media")
	class UMediaSource* MediaSource;
	
};
