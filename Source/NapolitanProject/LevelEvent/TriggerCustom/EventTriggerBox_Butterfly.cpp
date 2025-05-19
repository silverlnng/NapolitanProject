// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox_Butterfly.h"

#include "EngineUtils.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Event/Picture_Zombie.h"

void AEventTriggerBox_Butterfly::BeginPlay()
{
	Super::BeginPlay();
	BindBeginOverlap();

	
	if (!Picture_Zombie)
	{
		// 없으면 검색해서 채우기
		for (TActorIterator<APicture_Zombie> It(GetWorld(), APicture_Zombie::StaticClass()); It; ++It)
		{
			Picture_Zombie = *It;
		}
	}
}

void AEventTriggerBox_Butterfly::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (bHasTriggered)
		{
			return;
		}

		bHasTriggered = true;

		// 이때부터 복도의 좀비 캐릭터 실행되도록 하기
		if (Picture_Zombie)
		{
			Picture_Zombie->DropEffect();
		}
	}	
}
