// Fill out your copyright notice in the Description page of Project Settings.


#include "DigitLockActor.h"

#include "InteractWidget.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
ADigitLockActor::ADigitLockActor()
{
	
}

// Called when the game starts or when spawned
void ADigitLockActor::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
}

void ADigitLockActor::SetVisisbleInteractUI()
{
	if (PlayerHUD&&PlayerHUD->InteractUI)
	{
		PlayerHUD->InteractUI->SetVisibility(ESlateVisibility::Hidden);
	}
}


