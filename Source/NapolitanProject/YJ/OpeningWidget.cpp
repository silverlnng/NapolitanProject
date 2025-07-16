// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningWidget.h"
#include "LevelSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

void UOpeningWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
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
	
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld() , TicketSequence , FMovieSceneSequencePlaybackSettings() , outActor);
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
	this->SetVisibility(ESlateVisibility::Hidden);
	// 시퀀스 실행
	if (TicketSequence)
	{
		if (SequencePlayer)
		{
			SequencePlayer->Play();
		}
	}
}


