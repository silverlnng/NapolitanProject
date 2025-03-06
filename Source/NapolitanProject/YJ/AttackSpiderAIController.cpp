// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpiderAIController.h"

#include "AttackSpiderV2.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"

AAttackSpiderAIController::AAttackSpiderAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// AI PerceptionComponent 생성
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
}

void AAttackSpiderAIController::BeginPlay()
{
	Super::BeginPlay();
	
	AttackSpider=GetPawn<AAttackSpiderV2>();
	//AttackSpider=Cast<AAttackSpiderV2>(GetPawn());
	
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange_;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerception->ConfigureSense(*HearingConfig);
		AIPerception->SetDominantSense(UAISense_Hearing::StaticClass());
	}
	
	AIPerception->SetSenseEnabled(UAISense_Hearing::StaticClass(),true);
	
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAttackSpiderAIController::OnTargetPerceptionUpdated);
	
}

void AAttackSpiderAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == Stimulus.SensingSucceeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("👂 몬스터(AIController)가 플레이어 소리를 감지함! 위치: %s"),
			*Stimulus.StimulusLocation.ToString());
		if (Actor->IsA(ATestCharacter::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("캐릭터의 소리를 감지함")));

			//idle일때만 감지
			if (AttackSpider->CurrentState==EAttackSpiderV2State::Move)
			{
				AttackSpider->SetAIState(EAttackSpiderV2State::Drop);
			}
			
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("소리를 감지못함!")));
	}
}


void AAttackSpiderAIController::OnHearNoise(const TArray<AActor*>& Actor)
{
	// Actor 순회 .
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("소리를 감지함!")));
	
	for (auto &FoundActor  :Actor)
	{
		if (FoundActor->IsA(ATestCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("플레이어의 소리를 감지함!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("플레이어의 소리를 감지함!")));
			
			// 일정 거리 이내인지 확인
			/*float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());
			if (Distance <= HearingRange)
			{
				AttackPlayer();
			}*/
		}
	}
	
}