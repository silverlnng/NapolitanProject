// Fill out your copyright notice in the Description page of Project Settings.


#include "SouvenirWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

// curPage =0 일때는 Back 버튼 안보이도록 하기 

void USouvenirWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Souvenir_Back->OnClicked.AddDynamic(this,&USouvenirWidget::OnClicked_Souvenir_Back);
	Btn_Souvenir_front->OnClicked.AddDynamic(this,&USouvenirWidget::OnClicked_Souvenir_front);
}

void USouvenirWidget::OnClicked_Souvenir_Back()
{
	// 0,1 단위로 끊어서 읽기 
	// curPage-2 ,curPage-1 으로 나오게 하고
	// curPage-=2 으로 업데이트
	if(curPage-2<0){return;}
	curPage-=2;
	SouvenirBtn_BackDele.Broadcast(curPage);
	// 델리게이트 함수 실행 + 매개변수로 curPage 전달

	if (BookSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, BookSoundWave);
	}
}

void USouvenirWidget::OnClicked_Souvenir_front()
{
	// curPage+2 ,curPage+3 으로 나오게 하고
	//curPage+=2 으로 업데이트
	if(curPage+2>MaxPage){return;}
	curPage+=2;
	// 델리게이트 함수 실행 + 매개변수로 curPage 전달
	SouvenirBtn_BackDele.Broadcast(curPage);
	if (BookSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, BookSoundWave);
	}
}
