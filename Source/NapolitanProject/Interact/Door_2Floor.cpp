// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_2Floor.h"

#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void ADoor_2Floor::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor_2Floor::BeginOverlap);
}


void ADoor_2Floor::UnBindBeginOverlap()
{
	//청소부 만나고 나면 해제하기
	BoxComp->OnComponentBeginOverlap.Clear();
	// 문열리도록 하기 
	StartRotateDoor();
}

void ADoor_2Floor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
	// 독백대사 나오도록 하기
	if(OtherActor)
	{
		
		auto* MainCharacter = Cast<ATestCharacter>(OtherActor);

		//퀘스트 있는지 검사
		
		
	}
}

void ADoor_2Floor::RotateDoor()
{
	Super::RotateDoor();
}

void ADoor_2Floor::RotateDoor2()
{
	Super::RotateDoor2();
}

void ADoor_2Floor::StartRotateDoor()
{
	Super::StartRotateDoor();
}
