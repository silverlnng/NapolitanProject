// Fill out your copyright notice in the Description page of Project Settings.


#include "ESCWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"

void UESCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Exit->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Exit);
	Btn_StartLevel->OnClicked.AddDynamic(this,&UESCWidget::OnClick_StartLevel);
	Btn_Continue->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Continue);
	
	GI=GetGameInstance<UMyTestGameInstance>();
}

void UESCWidget::OnClick_Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UESCWidget::OnClick_StartLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),StartLevel,true);

	GI->SetLevelMoveToDoor(false); 
}

void UESCWidget::OnClick_Continue()
{
	SetVisibility(ESlateVisibility::Hidden);
}
