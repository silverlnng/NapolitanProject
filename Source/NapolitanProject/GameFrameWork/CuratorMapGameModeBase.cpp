// Fill out your copyright notice in the Description page of Project Settings.


#include "CuratorMapGameModeBase.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "TestCharacter.h"
#include "PlayerHUD.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"

ACuratorMapGameModeBase::ACuratorMapGameModeBase()
{
}

void ACuratorMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp , Warning , TEXT("Current Game Mode: %s") , *GetWorld()->GetAuthGameMode()->GetName());

	MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MainCharacter){return;}
	PC =MainCharacter->GetController<ATestPlayerController>();
	if (!PC){return;}
	PlayerHUD =PC->GetHUD<APlayerHUD>();	

	MainCharacter = Cast<ATestCharacter>(PC->GetPawn());
	MainCharacter->b_IA_Note_Allowed = true;
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

	//GI->ClearedNPC 와 NPCArray 를 비교해서 삭제
	if (!GI->ClearedNPC.IsEmpty())
	{
		for (int32 key :GI->ClearedNPC)
		{
			if (NPCArray.Contains(key))
			{
				NPCArray[key]->Destroy();
				// 어색 하면 Destroy()
			}
		}
	}

	if (GI)
	{
		FTimerHandle RestoreAttachedItemTimer;
		//아이템 인벤토리때문에 만든 것, 저장한 아이템과 인벤토리가 맵을 넘어가도 가져갈 수 있도록
		GetWorld()->GetTimerManager().SetTimer(RestoreAttachedItemTimer , [this]()
		{
			GI->RestoreAttachedItems();
		} , 1.0f , false);
		
		

		/*if (GI->bLevelMoveToDoor)
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
		}*/
	}
}
