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
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
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
	
	if (SaveGI->IsFromLoad)
	{
		// 그림으로 레벨이동 때문에 로드플레이 아닐때도 해야함 
		// GI->ClearedNPC 와 NPCArray 를 비교해서 삭제
		if (!SaveGI->ClearedNPC.IsEmpty())
		{
			for (int32 key :SaveGI->ClearedNPC)
			{
				if (NPCArray.Contains(key))
				{
					NPCArray[key]->Destroy();
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
		FTimerHandle RestoreAttachedItemTimer;
		GetWorld()->GetTimerManager().SetTimer(RestoreAttachedItemTimer , [this]()
		{
			SaveGI->RestoreAttachedItems();
			// 아이템을 인벤토리에  복구하는 작업
		} , 1.0f , false);
		
		if (SaveGI->bLevelMoveToDoor) // 그림으로 이동후 돌아올 때 
		{
			// 저장된 위치가 있으면 플레이어를 해당 위치로 이동
			FTimerHandle delayTimer;
			GetWorld()->GetTimerManager().SetTimer(delayTimer , [this]()
			{
				MainCharacter->SetActorLocation(SaveGI->GetSavedPlayerLocation().GetLocation());
				MainCharacter->SetActorRotation(SaveGI->GetSavedPlayerLocation().GetRotation());
				// 적용 후 다시 false로 변경 (새 게임 시작 시 영향 안 주도록)
				SaveGI->SetLevelMoveToDoor(false);
			} , 1.0f , false);
			
		}
		else  // 그림으로 이동후 돌아올 때는 아닐 때
		{
			
			FTimerHandle GITimer;
			GetWorld()->GetTimerManager().SetTimer(GITimer , [this]()
			{
				MainCharacter->SetActorLocation(SaveGI->LoadedGame->PlayerLocation);
				MainCharacter->SetActorRotation(SaveGI->LoadedGame->PlayerRotation);
			} , 1.0f , false);
		}
	}
	
	//// 설정값 로드하기 ->게임로드 안해도 일관성 유지하기 위해서
	LoadSettingValue();
}

void ATestGameModeBase::LoadSettingValue()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),MasterMix,MyMasterClass,SaveGI->TotalSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),BackGroundMix,BackGroundClass,SaveGI->BGSoundMix,1,0);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),SFXMix,MySFXClass,SaveGI->SFXSoundMix,1,0);
}


