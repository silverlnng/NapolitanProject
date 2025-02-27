// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"

#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"

ATestGameModeBase::ATestGameModeBase()
{
	
}

void ATestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp , Warning , TEXT("Current Game Mode: %s") , *GetWorld()->GetAuthGameMode()->GetName());

	PC = Cast<ATestPlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);

	MainCharacter = Cast<ATestCharacter>(PC->GetPawn());
	MainCharacter->b_IA_Note_Allowed = true;
	MainCharacter->b_IA_Inven_Allowed = true;

	PlayerHUD = PC->GetHUD<APlayerHUD>();

	GI = GetGameInstance<UMyTestGameInstance>();
	if (GI)
	{
		GI->RestoreAttachedItems();

		if (GI->bLevelMoveToDoor)
		{
			// 저장된 위치가 있으면 플레이어를 해당 위치로 이동
			MainCharacter->SetActorLocation(GI->GetSavedPlayerLocation().GetLocation());
			MainCharacter->SetActorRotation(GI->GetSavedPlayerLocation().GetRotation());
			
		}
		// 적용 후 다시 false로 변경 (새 게임 시작 시 영향 안 주도록)
		GI->SetLevelMoveToDoor(false);
	}
	else if (GI->LoadedGame)
	{
		FTimerHandle GITimer;

		GetWorld()->GetTimerManager().SetTimer(GITimer , [this]()
		{
			// GI->RestoreAttachedItems();

			MainCharacter->SetActorLocation(GI->LoadedGame->PlayerLocation);
			MainCharacter->SetActorRotation(GI->LoadedGame->PlayerRotation);
		} , 1.0f , false);
	}
}


