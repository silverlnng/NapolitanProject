// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStram1F_TriggerBox.h"

#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

void ALevelStram1F_TriggerBox::BeginPlay()
{
	Super::BeginPlay();
	BindBeginOverlap();
}

void ALevelStram1F_TriggerBox::BindBeginOverlap()
{
	Super::BindBeginOverlap();
}

void ALevelStram1F_TriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

		GetWorldTimerManager().SetTimer(LoadSubLevelTimerHandle, this, &ALevelStram1F_TriggerBox::ProcessNextSubLevel, 1.0f, false);
	}	
}

void ALevelStram1F_TriggerBox::ProcessNextSubLevel()
{
	Super::ProcessNextSubLevel();
}
