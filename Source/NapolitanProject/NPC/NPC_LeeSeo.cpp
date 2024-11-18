// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LeeSeo.h"

// Sets default values
ANPC_LeeSeo::ANPC_LeeSeo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_LeeSeo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_LeeSeo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC_LeeSeo::Interact()
{
	Super::Interact();
}

int32 ANPC_LeeSeo::GetNPCID()
{
	return NPC_ID;
}

int32 ANPC_LeeSeo::GetState()
{
	return Super::GetState();
}

void ANPC_LeeSeo::ChangeCleared()
{
	Super::ChangeCleared();
}

void ANPC_LeeSeo::SpawnItem()
{
	//아이템 스폰
}

void ANPC_LeeSeo::ResultEvent(int32 result)
{
	//결과 이벤트 작성
}
