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
		
		FLatentActionInfo LatentAction;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),LobbyLevel,true,true,LatentAction);

		// 시간지연을 주고
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			FLatentActionInfo LatentAction;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),CorriderLevel,true,true,LatentAction);
		}, 2.0f, false);

		FTimerHandle TimerHandle2;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [this]()
		{
			FLatentActionInfo LatentAction;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),LobbyRoom1Level,true,true,LatentAction);
		}, 6.0f, false);

		FTimerHandle TimerHandle3;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle3, [this]()
		{
			FLatentActionInfo LatentAction;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),LobbyRoom2Level,true,true,LatentAction);
		}, 8.0f, false);
		
		
	}	
}
