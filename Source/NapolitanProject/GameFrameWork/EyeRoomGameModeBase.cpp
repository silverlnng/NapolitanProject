// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeRoomGameModeBase.h"

#include "EngineUtils.h"
#include "MyTestGameInstance.h"
#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/Border.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/NPC/NPCCharacter.h"

AEyeRoomGameModeBase::AEyeRoomGameModeBase()
{
}

void AEyeRoomGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp , Warning , TEXT("Current Game Mode: %s") , *GetWorld()->GetAuthGameMode()->GetName());

	MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MainCharacter){return;}
	MainCharacter->GetMesh()->SetWorldScale3D(FVector3d(1,1,1));
	PC =MainCharacter->GetController<ATestPlayerController>();
	if (!PC){return;}
	PlayerHUD =PC->GetHUD<APlayerHUD>();	

	MainCharacter = Cast<ATestCharacter>(PC->GetPawn());
	MainCharacter->b_IA_Note_Allowed = true;
	MainCharacter->b_IA_Inven_Allowed = true;

	// 기존 바인딩 제거
	MainCharacter->OnSpecialInteraction.Clear();
	// 새로운 기능 바인딩
	MainCharacter->OnSpecialInteraction.AddDynamic(this, &AEyeRoomGameModeBase::Interaction_OnEyeRoomMap);
	
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
		//아이템 인벤토리때문에 만든 것, 저장한 아이템과 인벤토리가 맵을 넘어가도 가져갈 수 있도록
		GetWorld()->GetTimerManager().SetTimer(RestoreAttachedItemTimer , [this]()
		{
			GI->RestoreAttachedItems();
		} , 1.0f , false);
	}
}

void AEyeRoomGameModeBase::Interaction_OnEyeRoomMap(AActor* Interact)
{
	//자물쇠 태그 체크
	if (Interact->Tags.Contains(FName("Lock")))
	{
		MainCharacter->HandleLockInteraction();
	}
}
void AEyeRoomGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MainCharacter->OnSpecialInteraction.Clear();
}
