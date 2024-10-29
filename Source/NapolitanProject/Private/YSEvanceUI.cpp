// Fill out your copyright notice in the Description page of Project Settings.


#include "YSEvanceUI.h"
#include "Components/TextBlock.h"

void UYSEvanceUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UYSEvanceUI::PlayAnim()
{
	PlayAnimationForward(TextAnim);
}
