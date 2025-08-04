// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningWidget.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/RetainerBox.h"
#include "Kismet/GameplayStatics.h"

void UOpeningWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Media_CanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	
	// 첫 번째 애니메이션이 끝났을 때 실행될 델리게이트 바인딩
	Anim0nFinishedDelegate.BindDynamic(this, &UOpeningWidget::OnFirstAnimationFinished);
	Anim1nFinishedDelegate.BindDynamic(this, &UOpeningWidget::OnSecondAnimationFinished);
	Anim2nFinishedDelegate.BindDynamic(this, &UOpeningWidget::OnThirdAnimationFinished);
	Anim3nFinishedDelegate.BindDynamic(this, &UOpeningWidget::OnFourthAnimationFinished);
	Anim4nFinishedDelegate.BindDynamic(this, &UOpeningWidget::OnfifthAnimationFinished);
	
	BindToAnimationFinished(TextAnim_0,Anim0nFinishedDelegate);
	BindToAnimationFinished(TextAnim_1,Anim1nFinishedDelegate);
	BindToAnimationFinished(TextAnim_2,Anim2nFinishedDelegate);
	BindToAnimationFinished(TextAnim_3,Anim3nFinishedDelegate);
	BindToAnimationFinished(TextAnim_4,Anim4nFinishedDelegate);
	
	// 첫 번째 애니메이션 재생
	PlayAnimation(TextAnim_0);
	// 소리 재생
	if (TTSSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, TTSSoundWave);
	}
}

void UOpeningWidget::OnFirstAnimationFinished()
{
	// 첫 번째 애니메이션 재생
	PlayAnimation(TextAnim_1);

}

void UOpeningWidget::OnSecondAnimationFinished()
{
	// 첫 번째 애니메이션 재생
	PlayAnimation(TextAnim_2);
}

void UOpeningWidget::OnThirdAnimationFinished()
{
	// 첫 번째 애니메이션 재생
	PlayAnimation(TextAnim_3);
}

void UOpeningWidget::OnFourthAnimationFinished()
{
	// 첫 번째 애니메이션 재생
	PlayAnimation(TextAnim_4);
}

void UOpeningWidget::OnfifthAnimationFinished()
{
	// 자기자신은 hidden 처리
	RetainerBox->SetVisibility(ESlateVisibility::Hidden);

	Media_CanvasPanel->SetVisibility(ESlateVisibility::Visible);
	
	// 미디어 플레이실행
	if (MediaPlayer && MediaSource)
	{
		// MediaSource 열기 시도
		if (MediaPlayer->OpenSource(MediaSource))
		{
			UE_LOG(LogTemp, Log, TEXT("Media source opened successfully!"));
			MediaPlayer->Play(); // 자동 재생
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to open media source!"));
		}
	}
}


