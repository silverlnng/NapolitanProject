// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpiderV2.h"

#include "AIController.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

// Sets default values
AAttackSpiderV2::AAttackSpiderV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());

	// AIPerceptionComponent 생성
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// 청각 감지 설정
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;


		
		// 🎯 감지 범위를 3D Sphere처럼 만들기
		HearingConfig->HearingRange=FMath::Sqrt(HearingRange * HearingRange + HearingZRange * HearingZRange);
		
		AIPerception->ConfigureSense(*HearingConfig);
		AIPerception->SetDominantSense(UAISense_Hearing::StaticClass());
		
		AIPerception->ConfigureSense(*HearingConfig);
		AIPerception->SetDominantSense(UAISense_Hearing::StaticClass());
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAttackSpiderV2::OnHearNoise);
	}
}

// Called when the game starts or when spawned
void AAttackSpiderV2::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	if (SplineActor)
	{
		CurrentSpline=Cast<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	}
	if (CurrentSpline==nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentSpline Null")));
	}
	
	StartMoving();
}

// Called every frame
void AAttackSpiderV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving && CurrentSpline)
	{
		MoveAlongSpline(DeltaTime);
	}
}

void AAttackSpiderV2::MoveAlongSpline(float DeltaTime)
{
	if (!CurrentSpline) return;

	// 이동 거리 증가
	DistanceAlongSpline += MoveSpeed * DeltaTime;
	
	float SplineLength = CurrentSpline->GetSplineLength();

	// 🎯 Spline 끝에 도달하면 처음으로 되돌리기
	if (DistanceAlongSpline >= SplineLength)
	{
		DistanceAlongSpline = 0.0f; // 🎯 다시 시작점으로 이동
	}

	// 새로운 위치 및 회전 계산
	FVector NewLocation = CurrentSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator NewRotation = CurrentSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AAttackSpiderV2::StartMoving()
{
	if (CurrentSpline)
	{
		DistanceAlongSpline = 0.0f;
		bIsMoving = true;
	}
}

void AAttackSpiderV2::AttackPlayer()
{
	//움직임을 멈추고
	bIsMoving=false;

	// 공격시작
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("공격시작")));
}

void AAttackSpiderV2::OnHearNoise(const TArray<AActor*>& Actor)
{
	// Actor 순회 .
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("소리를 감지함!")));
	
	for (auto &FoundActor  :Actor)
	{
		if (FoundActor->IsA(ATestCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("플레이어의 소리를 감지함!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("플레이어의 소리를 감지함!")));
			
			// 일정 거리 이내인지 확인
			float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());
			if (Distance <= HearingRange)
			{
				AttackPlayer();
			}
		}
	}
	
}

