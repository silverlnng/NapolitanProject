// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectiveMapGameModeBase.h"

#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/Border.h"
#include "NapolitanProject/Interact/InteractWidget.h"

void ADetectiveMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PC=Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
	
	MainCharacter=Cast<ATestCharacter>(PC->GetPawn());
	// Q,R 키를 막기. 
	MainCharacter->b_IA_Note_Allowed=false;
	MainCharacter->b_IA_Inven_Allowed=false;
	
	PlayerHUD =PC->GetHUD<APlayerHUD>();

	FTimerHandle GITimer;

	GetWorld()->GetTimerManager().SetTimer(GITimer,[this]()
	{
		PlayerHUD->InteractUI->Border_Inven->SetVisibility(ESlateVisibility::Hidden);
	},0.2f,false);

	// state 에 따라서 구분되도록 하기

	// 스폰되는 게 달라야함 . 
	
	switch (DetectiveMapState)
	{
	case EDetectiveMapState::FisrtStart:
		break;
	case EDetectiveMapState::FirstEnding:
		break;
	case EDetectiveMapState::SecondEnding:
		break;
	}
}
