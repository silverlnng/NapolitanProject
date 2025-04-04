// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox_Docent.h"

#include "EngineUtils.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Docent_JumpScare.h"


void AEventTriggerBox_Docent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!Docent_JumpScare)
	{
		// 없으면 검색해서 채우기
		for (TActorIterator<ADocent_JumpScare> It(GetWorld(), ADocent_JumpScare::StaticClass()); It; ++It)
		{
			Docent_JumpScare = *It;
		}
	}
}

void AEventTriggerBox_Docent::BindBeginOverlap()
{
	Super::BindBeginOverlap();
}

void AEventTriggerBox_Docent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->IsA(ATestCharacter::StaticClass()))
	{
		if (bHasTriggered)
		{
			return;
		}

		bHasTriggered = true; 
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("EventTriggerBox_Docent::BeginOverlap")));
		// 점프스케어용 도슨트가 걸어나오고
		if (Docent_JumpScare)
		{
			Docent_JumpScare->SetActorHiddenInGame(false);
			Docent_JumpScare->StartMoving();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Docent_JumpScare_StartMoving")));
		}
		//  도슨트 점프스케어 시작. 
	}
}
