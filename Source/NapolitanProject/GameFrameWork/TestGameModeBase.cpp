// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "SaveGISubsystem.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/Interact/PieceActor.h"
#include "NapolitanProject/Interact/Sculpture.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"

ATestGameModeBase::ATestGameModeBase()
{
	
}

void ATestGameModeBase::StartPlay()
{
	Super::StartPlay();
	//GI = GetGameInstance<UMyTestGameInstance>();
	//GI->LoadingScreenWidget
	
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
	MainCharacter->b_IA_Inven_Allowed = true;

	PlayerHUD = PC->GetHUD<APlayerHUD>();
	GI = GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
	
	// 현재 맵에 있는 npc들을 저장
	for (TActorIterator<ANPCCharacter> It(GetWorld(), ANPCCharacter::StaticClass()); It; ++It)
	{
		ANPCCharacter* NPC=*It;
		int32 key = NPC->GetNPCID();
		NPCArray.Add(key,NPC);
		// 로그 출력으로 확인하기
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NPCArray: %d"),key));
	}	
	

	// GI->ClearedNPC 와 NPCArray 를 비교해서 삭제
	
	if (!SaveGI->ClearedNPC.IsEmpty())
	{
		for (int32 key :SaveGI->ClearedNPC)
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
			if (2==key) // 도슨트 클리어 => 
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
	
	for (TActorIterator<AItemActor> It(GetWorld()); It; ++It)
	{
		AItemActor* Item=*It;
		int32 key = Item->GetItemID();
		ItemActorArray.Add(key,Item);
		// 로그 출력으로 확인하기
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ItemActorArray: %d"),key));
		UE_LOG(LogTemp, Error, TEXT("%s ItemActorArray: %d"),*CALLINFO,key);
	}
	
	if (SaveGI)
	{
		if (SaveGI->bLevelMoveToDoor) // 그림으로 이동후 돌아올 때 
		{
			// 저장된 위치가 있으면 플레이어를 해당 위치로 이동
			MainCharacter->SetActorLocation(SaveGI->GetSavedPlayerLocation().GetLocation());
			MainCharacter->SetActorRotation(SaveGI->GetSavedPlayerLocation().GetRotation());
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				FLatentActionInfo LatentAction;
				for (auto subLevel:SaveGI->SubLevelArray)
				{
					UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),subLevel,true,true,LatentAction);
				}
			
			}, 1.0f, false);
			
			
			// 적용 후 다시 false로 변경 (새 게임 시작 시 영향 안 주도록)
			SaveGI->SetLevelMoveToDoor(false);
			
		}
		else if (SaveGI->LoadedGame) // 로드플레이 중 이다
		{
			
			FTimerHandle GITimer;
			GetWorld()->GetTimerManager().SetTimer(GITimer , [this]()
			{
				MainCharacter->SetActorLocation(SaveGI->LoadedGame->PlayerLocation);
				MainCharacter->SetActorRotation(SaveGI->LoadedGame->PlayerRotation);
			} , 1.0f , false);
			
			FTimerHandle GITimer2;
			GetWorld()->GetTimerManager().SetTimer(GITimer2 , [this]()
			{
				// 적용 후 다시 nullptr 변경 (새 게임 시작 시 영향 안 주도록)
				SaveGI->LoadedGame=nullptr;
			} , 2.5f , false);

		}
		
		FTimerHandle RestoreAttachedItemTimer;
		GetWorld()->GetTimerManager().SetTimer(RestoreAttachedItemTimer , [this]()
		{
			SaveGI->RestoreAttachedItems();
			// 아이템을 인벤토리에  복구하는 작업
		} , 1.0f , false);

		// 인벤에 이미 있는 아이템은 맵에서 제거하는 함수
		FTimerHandle RestoreItemTimer;
		GetWorld()->GetTimerManager().SetTimer(RestoreItemTimer , [this]()
		{
			// 여기서 수정을 해야함 . 내가 저장한 아이템에 4,5이 있는지 더 나은 방법으로 
			AItemActor* Item4 =PlayerHUD->InventoryUI->InvenSlots[4]->MyItem;
			//
			if (Item4) // MyItem이 있으면 획득했었다는 의미
			{
				 // 빵 아이템 hidden 처리
				//BreadItem4->SetActorHiddenInGame(true);
				if (ItemActorArray.Contains(4))
				{
					ItemActorArray[4]->SetActorHiddenInGame(true);
				}
				
			}
			
			AItemActor* Item5 =PlayerHUD->InventoryUI->InvenSlots[4]->MyItem;
			if (Item5) // MyItem이 있으면 획득했었다는 의미
			{
				 // 빵 아이템 hidden 처리
				//BreadItem5->SetActorHiddenInGame(true);
				if (ItemActorArray.Contains(5))
				{
					ItemActorArray[5]->SetActorHiddenInGame(true);
				}
				
			}
			
			
		} , 1.5f , false);
		

		//// 설정값 로드하기 
		LoadSettingValue();
	}
}

void ATestGameModeBase::LoadSettingValue()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),MasterMix,MyMasterClass,SaveGI->TotalSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),BackGroundMix,BackGroundClass,SaveGI->BGSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),SFXMix,MySFXClass,SaveGI->SFXSoundMix,1,0);
}


