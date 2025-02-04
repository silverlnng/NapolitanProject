// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../YJ/NoteUI/InventoryWidget.h"
#include "../YJ/NoteUI/InvenSlotWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../YJ/NoteUI/NoteWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
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
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::OnPickup()
{
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	OnInventorySlot();
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// 해당하는 아이템슬롯을 찾아서 자신을 넣어주기
	if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
	{
		PlayerHUD->InventoryUI->InvenSlots[ItemID]->MyItem=this;
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
	// 게임인스턴스의 데이터 테이블도 had로 변경시키면 기록될것
	// 본인 아이템 ItemID 으로 슬롯 찾아서 활성화 되도록 만들기
	if (PlayerHUD->InventoryUI)
	{
		if (PlayerHUD->InventoryUI->InvenSlots.Contains(ItemID))
		{
			PlayerHUD->InventoryUI->InvenSlots[ItemID]->OnItemAcquired();
		}
	}
}

