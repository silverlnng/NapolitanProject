// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../YJ/NoteUI/InventoryWidget.h"
#include "../YJ/NoteUI/InvenSlotWidget.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "../YJ/NoteUI/NoteWidget.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PlayerHUD=PC->GetHUD<APlayerHUD>();
	}
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::OnPickup()
{
	// 본인 아이템 ItemID 으로 슬롯 찾아서 활성화 되도록 만들기
	PlayerHUD->NoteUI->WBP_Inventory->InvenSlots[ItemID]->OnItemAcquired();
	// 게임인스턴스의 데이터 테이블도 had로 변경시키면 기록될것
}

