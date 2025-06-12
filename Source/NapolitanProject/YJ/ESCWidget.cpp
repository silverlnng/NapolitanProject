// Fill out your copyright notice in the Description page of Project Settings.


#include "ESCWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"

void UESCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Exit->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Exit);
	Btn_StartLevel->OnClicked.AddDynamic(this,&UESCWidget::OnClick_StartLevel);
	Btn_Continue->OnClicked.AddDynamic(this,&UESCWidget::OnClick_Continue);
	
	GI=GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
}

void UESCWidget::OnClick_Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UESCWidget::OnClick_StartLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),StartLevel,true);

	SaveGI->SetLevelMoveToDoor(false); 
}

void UESCWidget::OnClick_Continue()
{
	SetVisibility(ESlateVisibility::Hidden);
}
