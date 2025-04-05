// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/Border.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interact/PieceActor.h"
#include "NapolitanProject/Interact/Sculpture.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
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
	MainCharacter->GetMesh()->SetWorldScale3D(FVector3d(1,1,1));
	//MainCharacter->b_IA_Note_Allowed = true; => 도슨트 클리어하고 될수있도록 하기 
	MainCharacter->b_IA_Inven_Allowed = true;

	PlayerHUD = PC->GetHUD<APlayerHUD>();
	
	// 현재 맵에 있는 npc들을 저장
	for (TActorIterator<ANPCCharacter> It(GetWorld(), ANPCCharacter::StaticClass()); It; ++It)
	{
		ANPCCharacter* NPC=*It;
		int32 key = NPC->GetNPCID();
		NPCArray.Add(key,NPC);
		// 로그 출력으로 확인하기
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NPCArray: %d"),key));
	}	

	GI = GetGameInstance<UMyTestGameInstance>();

	// GI->ClearedNPC 와 NPCArray 를 비교해서 삭제
	
	if (!GI->ClearedNPC.IsEmpty())
	{
		for (int32 key :GI->ClearedNPC)
		{
			if (NPCArray.Contains(key))
			{
				NPCArray[key]->Destroy();
				// 어색 하면 Destroy()
			}

			if (4==key) // 경비원 npc클리어했을때 
			{
				// 2층의 조각상 삭제
				for (TActorIterator<ASculpture> It(GetWorld(), ASculpture::StaticClass()); It; ++It)
				{
					ASculpture* Sculpture = *It;
					Sculpture->Destroy(1);
				}
				for (TActorIterator<APieceActor> It(GetWorld(), APieceActor::StaticClass()); It; ++It)
				{
					APieceActor* Piece = *It;
					Piece->Destroy(1);
				}
			}
			if (2==key) // 도슨트
			{
				MainCharacter->b_IA_Note_Allowed = true;
				FTimerHandle HUDTimer;
				GetWorld()->GetTimerManager().SetTimer(HUDTimer , [this]()
				{
					PlayerHUD->InteractUI->Border_Note->SetVisibility(ESlateVisibility::Visible);
				} , 1.5f , false);
			}
			
		}
	}
	
	
	if (GI)
	{
		FTimerHandle RestoreAttachedItemTimer;

		GetWorld()->GetTimerManager().SetTimer(RestoreAttachedItemTimer , [this]()
		{
			GI->RestoreAttachedItems();
		} , 1.0f , false);
		
		if (GI->bLevelMoveToDoor)
		{
			// 저장된 위치가 있으면 플레이어를 해당 위치로 이동
			MainCharacter->SetActorLocation(GI->GetSavedPlayerLocation().GetLocation());
			MainCharacter->SetActorRotation(GI->GetSavedPlayerLocation().GetRotation());
			
			// 적용 후 다시 false로 변경 (새 게임 시작 시 영향 안 주도록)
			GI->SetLevelMoveToDoor(false);
		}
		else if (GI->LoadedGame)
		{
			
			FTimerHandle GITimer;
			GetWorld()->GetTimerManager().SetTimer(GITimer , [this]()
			{
				MainCharacter->SetActorLocation(GI->LoadedGame->PlayerLocation);
				MainCharacter->SetActorRotation(GI->LoadedGame->PlayerRotation);
			} , 1.0f , false);
			
			FTimerHandle GITimer2;
			GetWorld()->GetTimerManager().SetTimer(GITimer2 , [this]()
			{
				// 적용 후 다시 nullptr 변경 (새 게임 시작 시 영향 안 주도록)
				GI->LoadedGame=nullptr;
			} , 2.5f , false);

		}
	}
	
}


