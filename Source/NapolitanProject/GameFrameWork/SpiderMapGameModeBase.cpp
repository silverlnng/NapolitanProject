// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderMapGameModeBase.h"

#include "PlayerHUD.h"
#include "TestCharacter.h"
#include "TestPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NapolitanProject/Interact/CatchSpider.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"

ASpiderMapGameModeBase::ASpiderMapGameModeBase()
{
	
	
}

void ASpiderMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter=Cast<ATestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PC =MainCharacter->GetController<ATestPlayerController>();
	PlayerHUD =PC->GetHUD<APlayerHUD>();
	
	if (MainCharacter)
	{
		// 기존 바인딩 제거
		MainCharacter->OnSpecialInteraction.Clear();

		// 새로운 기능 바인딩
		MainCharacter->OnSpecialInteraction.AddDynamic(this, &ASpiderMapGameModeBase::Interaction_OnSpiderMap);
		
	}
	if (SpiderBP)
	{
		//SpiderItem=SpiderBP->GetDefaultObject<AItemActor>();
		SpiderItem = GetWorld()->SpawnActor<AItemActor>(SpiderBP);
	}
	
}

void ASpiderMapGameModeBase::Interaction_OnSpiderMap(AActor* Interact)
{
	// 현재 거미잡이 총을 잡고있고 거미를 향해 E키를 눌렀을경우4
	// MainCharacter->curItem->ItemID 
	ACatchSpider* CatchSpider =Cast<ACatchSpider>(Interact);
		
	if (CatchSpider)
	{
		

		// 갯수도 누적해서 표시.
		CatchSpiderCount++;
		
		FString CatchSpiderNum=FString::FromInt(CatchSpiderCount);

		if (CatchSpiderCount==1)
		{
			SpiderItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
			SpiderItem->AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
			SpiderItem->SetActorHiddenInGame(true);
			
			if (PlayerHUD->InventoryUI)
			{
				if (PlayerHUD->InventoryUI->InvenSlots.Contains(SpiderItemID))
				{
					PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->OnItemAcquired();
					// 아이템 슬롯 작업.
					PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->Set_TextNum(CatchSpiderNum);
				}
			}
			// 해당하는 아이템슬롯을 찾아서 자신을 넣어주기
			if (PlayerHUD->InventoryUI->InvenSlots.Contains(SpiderItemID))
			{
				PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->MyItem=SpiderItem;
			}
		}
		else
		{
			PlayerHUD->InventoryUI->InvenSlots[SpiderItemID]->Set_TextNum(CatchSpiderNum);
		}
	}
	
}

void ASpiderMapGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MainCharacter->OnSpecialInteraction.Clear();
}
