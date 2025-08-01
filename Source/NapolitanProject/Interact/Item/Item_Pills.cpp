// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Pills.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NapolitanProject/YJ/NoteUI/InventoryWidget.h"

void AItem_Pills::BeginPlay()
{
	Super::BeginPlay();

	// 처음시작하자마자 인벤토리 안에 있게 하고싶음 
	
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// 인벤 작업 시간지연 주기 
	// 인벤토리 슬롯 작업중 UI 이벤트만 제외하고 실행기키기 
	// 본인 아이템 ItemID 으로 슬롯 찾아서 활성화 되도록 만들기
	
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer,[this]()
	{
		if (PlayerHUD->InventoryUI)
		{
			if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
			{
				PlayerHUD->InventoryUI->InvenSlots[ItemID]->OnItemAcquired();
			}
		}
		// 해당하는 아이템슬롯을 찾아서 자신을 넣어주기 ==> 이걸 초기화 때 하는게 편할수도 있음 
		if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
		{
			PlayerHUD->InventoryUI->InvenSlots[ItemID]->MyItem=this;
		}

		int32 ItemRow =ItemID;
	
		if (GI->itemDataMap.Contains(ItemRow))
		{
			FString ItemName=GI->itemDataMap[ItemRow].Name;
			GI->itemDataMap[ItemRow].Had=true;
		}

		SetActorHiddenInGame(true); // 인벤에 넣을 아이템은 일단 안보이도록

		//인벤에 넣었으면 다른거 집을수있도록
		MainCharacter->curItem=nullptr;

		SaveGI->SavedItems.Add(this->GetClass());
	},1.5f,false);
	
}
