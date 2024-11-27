// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalGameEndingWidget.h"

void UNormalGameEndingWidget::PlayEndingAnim()
{
	if (EndingAnim)
	{
		// 애니메이션 재생
		PlayAnimation(EndingAnim);
	}
}
