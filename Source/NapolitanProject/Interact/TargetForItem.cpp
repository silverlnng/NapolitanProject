// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetForItem.h"

#include "ItemActor.h"

// Sets default values
ATargetForItem::ATargetForItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetForItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetForItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetForItem::CheckItem(class AItemActor* curItem)
{
	// curItem 을 검증해서 작동되도록 만들기
	if (curItem->ItemID==TargetItemID)
	{
		CheckItemSuccess();
	}
	else
	{
		// 나와 작용하는 아이템이 아님 .
		// ui 나오도록 하고 .
		return;
	}
	
}

void ATargetForItem::CheckItemSuccess()
{
}
