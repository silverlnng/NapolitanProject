// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFloorDoorToLobby.h"

ASunFloorDoorToLobby::ASunFloorDoorToLobby()
{
	
}

void ASunFloorDoorToLobby::BeginPlay()
{
	Super::BeginPlay();

	bOneMove = false;
	
}

void ASunFloorDoorToLobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASunFloorDoorToLobby::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//조건 걸기
	if (!bOneMove) return; // 한번만 이동할 수 있도록
	
	Super::BeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
}
