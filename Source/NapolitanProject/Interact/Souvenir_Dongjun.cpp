// Fill out your copyright notice in the Description page of Project Settings.


#include "Souvenir_Dongjun.h"

#include "ItemActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"

// Sets default values
ASouvenir_Dongjun::ASouvenir_Dongjun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASouvenir_Dongjun::BeginPlay()
{
	Super::BeginPlay();

	if (RoseItemBP)
	{
		//SpiderItem=SpiderBP->GetDefaultObject<AItemActor>();
		RoseItem = GetWorld()->SpawnActor<AItemActor>(RoseItemBP);
	}
	
}

// Called every frame
void ASouvenir_Dongjun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ASouvenir_Dongjun::GetSouvenirID()
{
	return this->SouvenirID;
}

FString ASouvenir_Dongjun::GetSouvenirName()
{
	return this->SouvenirName;
}

void ASouvenir_Dongjun::OnPickup()
{
	Super::OnPickup();
	// 아이템처럼 들어오는 작업도 수행하기
	if (!RoseItem) {return;}
	RoseItem->BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	RoseItem->AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
	RoseItem->SetActorHiddenInGame(true);
	RoseItem->OnInventorySlot();
	
	/*if (PlayerHUD->InventoryUI)
	{
		if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
		{
			PlayerHUD->InventoryUI->InvenSlots[ItemID]->OnItemAcquired();
			// 아이템 슬롯 작업.
		}
	}
	// 해당하는 아이템슬롯을 찾아서 자신을 넣어주기
	if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
	{
		PlayerHUD->InventoryUI->InvenSlots[ItemID]->MyItem=RoseItem;
	}*/
	
}

