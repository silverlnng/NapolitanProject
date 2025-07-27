// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGISubsystem.h"

#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/Interact/Item/ItemActor.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"
#include "NapolitanProject/YJ/Save/GameSaveController.h"

void USaveGISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// 저장한 게임 저장데이터가 있으면 불러오기  ==> 이걸 스타트화면이 불러올때마다 하기 
	GameSaveController = NewObject<UGameSaveController>(this);
	int32 MaxSlots = 3; // 예: 최대 슬롯 수
	SaveSlotInfos = GameSaveController->LoadAllSlotInfo(MaxSlots);
	// 로드한 정보로 ULoadScreenWidget 초기화
}

void USaveGISubsystem::SaveAttachedItems()
{
	ATestCharacter* MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void USaveGISubsystem::RestoreAttachedItems()
{
	ATestCharacter* MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD());
	
	if (!MainCharacter || SavedItems.Num() == 0) return;
	for (const TSubclassOf<class AItemActor> ItemActor: SavedItems)
	{
		if (ItemActor)
		{
			AItemActor* NewItem = MainCharacter->GetWorld()->SpawnActor<AItemActor>(ItemActor);
			// ✅ 아이템 스폰

			// 스폰아이템은 태그달아주기 
			NewItem->Tags.Add(FName("SpawnedFromSave"));
			
			NewItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
			
			NewItem->AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
			NewItem->SetActorHiddenInGame(true); // 인벤에 넣을 아이템은 일단 안보이도록
			UE_LOG(LogTemp,Warning,TEXT("%s Load ItemID :%d"),*CALLINFO,NewItem->ItemID);
			if (PlayerHUD)
			{
				if (PlayerHUD->InventoryUI->InvenSlots.Contains(NewItem->ItemID))
				{
					PlayerHUD->InventoryUI->InvenSlots[NewItem->ItemID]->MyItem=NewItem;
					PlayerHUD->InventoryUI->InvenSlots[NewItem->ItemID]->OnItemAcquired();
				}
				if (NewItem->ItemID==3) // 거미일때만
				{
					PlayerHUD->InventoryUI->InvenSlots[3]->Set_TextNum(CatchSpiderNum);
					UE_LOG(LogTemp,Warning,TEXT("%sCatchSpider :%s"),*CALLINFO,*CatchSpiderNum);
				}
				// 이미 빵그림을 가졌으면 월드에 있는 빵은 hidden 처리를 하기  
			}
			
		}
	}	
}

void USaveGISubsystem::SavePlayerFTransform(FTransform NewLocation)
{
	SavedPlayerTransform = NewLocation;
	bLevelMoveToDoor = true; // 문을 통해 이동했으므로 저장된 위치 사용
}

FTransform USaveGISubsystem::GetSavedPlayerLocation() const
{
	return SavedPlayerTransform;
}

void USaveGISubsystem::SetLevelMoveToDoor(bool bUse)
{
	bLevelMoveToDoor = bUse; // 문을 통해 이동했으므로 저장된 위치 사용
	// 죽어서 처음레벨 가면 이거 false으로 고치기 
}

void USaveGISubsystem::LoadSettingValue()
{
	
}

