// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

#include "InteractWidget.h"
#include "../YJ/NoteUI/InventoryWidget.h"
#include "../YJ/NoteUI/InvenSlotWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../YJ/NoteUI/NoteWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);
	
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	GI =GetGameInstance<UMyTestGameInstance>();
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();

	
	
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	if(M_Overlay)
	{
		StaticMeshComp->SetOverlayMaterial(M_Overlay);
	}

	// 게임 저장/로드시
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer,[this]()
	{
		if (SaveGI->IsFromLoad&& !Tags.Contains("SpawnedFromSave"))
		{
			if (SaveGI->SavedItems.Contains(this->GetClass()))
			{
				Destroy();
			}
		}
	},1.f,false);
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::IInteract()
{
	if (MainCharacter->curItem) { return; }
	MainCharacter->curItem = this;
	OnPickup();
}

void AItemActor::OnPickup() // 아이템을 레벨에서 처음한번 잡을때. 인벤작업까지 되어있음
{
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	OnInventorySlot(); // 인벤토리 슬롯 작업

	SetActorHiddenInGame(true); // 인벤에 넣을 아이템은 일단 안보이도록

	//인벤에 넣었으면 다른거 집을수있도록
	MainCharacter->curItem=nullptr;

	SaveGI->SavedItems.Add(this->GetClass());

	// 사운드 재생시키기 
	if (ItemSoundWave)
	{
		UGameplayStatics::PlaySound2D(this, ItemSoundWave);
	}
}

void AItemActor::Use()
{
	
}

void AItemActor::PutDown()
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	SetActorHiddenInGame(true);
	Destroy();
}

void AItemActor::OnInventorySlot()
{
	
	// 본인 아이템 ItemID 으로 슬롯 찾아서 활성화 되도록 만들기
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
	// 인벤 효과 애니메이션 실행시키기 
	PlayerHUD->InteractUI->PlayInvenUIEvent();
	
	int32 ItemRow =ItemID;
	
	if (GI->itemDataMap.Contains(ItemRow))
	{
		FString ItemName=GI->itemDataMap[ItemRow].Name;
		PlayerHUD->InteractUI->GetItemEvent(ItemName);
		GI->itemDataMap[ItemRow].Had=true;
	}
}

void AItemActor::Remove()
{
	int32 ItemRow =ItemID;
	
	if (GI->itemDataMap.Contains(ItemRow))
	{
		GI->itemDataMap[ItemRow].Had=false;
	}
	
	if (SaveGI->SavedItems.Contains(this->GetClass()))
	{
		SaveGI->SavedItems.Remove(this->GetClass());
	}

	// 버튼도 비활성화 하기
	// 본인 아이템 ItemID 으로 슬롯 찾아서 활성화 되도록 만들기
	if (PlayerHUD->InventoryUI)
	{
		if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
		{
			PlayerHUD->InventoryUI->InvenSlots[ItemID]->SetIsEnabledBtn();
		}
	}
}

int32 AItemActor::GetItemID()
{
	return ItemID;
}

