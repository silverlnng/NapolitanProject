// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleTrigger_Curator.h"

#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"


void ASimpleTrigger_Curator::BeginPlay()
{
	Super::BeginPlay();
	if (ZombieMutant)
	{
		ZombieMutant->SetActorHiddenInGame(true);
	}
}

void ASimpleTrigger_Curator::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(ATestCharacter::StaticClass())) return;

	// 테스트 용으로 그냥 연출하기 
	/*if (bOnlyOnce){return;}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("ASimpleTrigger_Curator"));
	bOnlyOnce=true;
	// 문이열리면서 점프스케어 튀어나오기
	ZombieMutant->SetActorHiddenInGame(false);
	ZombieMutant->PlayAnimMontage(attackAM);*/


	
	// 큐레이터를 만난후인지 체크하기
	
	FName eventKey =TEXT("CuratorCompleted");
	if (GI->QuestCommandsMap.Contains(eventKey))
	{
		if (GI->QuestCommandsMap[eventKey].Done)
		{
			// 오직한번만 
			if (bOnlyOnce){return;}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("ASimpleTrigger_Curator"));
			bOnlyOnce=true;
			// 문이열리면서 점프스케어 튀어나오기
			ZombieMutant->SetActorHiddenInGame(false);
			ZombieMutant->PlayAnimMontage(attackAM);
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Not CuratorCompleted"));
			return;
		}
	}
	
	
}
