// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStram2F_TriggerBox.h"

#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void ALevelStram2F_TriggerBox::BeginPlay()
{
	Super::BeginPlay();
	BindBeginOverlap();
}

void ALevelStram2F_TriggerBox::BindBeginOverlap()
{
	Super::BindBeginOverlap();
}

void ALevelStram2F_TriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 로비레벨 로드되는거 실행하기
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (bHasTriggered)
		{
			return;
		}

		bHasTriggered = true;
		if (SecondFloor_Level)
		{
			FLatentActionInfo LatentAction;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),SecondFloor_Level,true,true,LatentAction);
		}
		
	}	
	
}
