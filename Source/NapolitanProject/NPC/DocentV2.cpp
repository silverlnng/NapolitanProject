// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentV2.h"

#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
ADocentV2::ADocentV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADocentV2::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}

	ChoseRandomTimeTurnRightAnim();
}

// Called every frame
void ADocentV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanDetectMovement && MainCharacter)
	{
		DetectPlayerMovement();
	}
	
}


// ChoseRandomTimeTurnRightAnim 시작자체를
void ADocentV2::ChoseRandomTimeTurnRightAnim()
{
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay = 4.0f;
	}
	else
	{
		ChosenDelay = FMath::RandBool() ? 2.0f : 3.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("선택된 딜레이: %.1f초"), ChosenDelay);

	
	FTimerHandle PlayTurnRightAnim;
	GetWorld()->GetTimerManager().SetTimer(PlayTurnRightAnim,[this]()
	{
		PlayTurnRightAnimation();
	},ChosenDelay,false);
	
}

void ADocentV2::PlayTurnRightAnimation()
{
	
	GetMesh()->PlayAnimation(TurnAroundMontage,false);
	
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay = 8.0f;
	}
	else
	{
		ChosenDelay = 6.0f;
	}

	// 0.5초 지난후 부터 플레이어의 움직임을 감지
	GetWorldTimerManager().SetTimer(
		  StartDetectionTimerHandle,
		  this,
		  &ADocentV2::StartMovementDetection,
		  2.0f,
		  false
	  );
	
	// 그리고 다시 원래대로 회전도 해야함 + 플레이어 감지 스탑

	GetWorld()->GetTimerManager().SetTimer(StopDetectionTimerHandle,[this]()
	{
		PlayTurnOriginAnimation();
		StopMovementDetection();
	},ChosenDelay,false);
}

void ADocentV2::PlayTurnOriginAnimation()
{
	if (TurnOriginMontage)
	{
		GetMesh()->PlayAnimation(TurnOriginMontage,false);
	}

	// 다시 시작
	ChoseRandomTimeTurnRightAnim();
}

void ADocentV2::StartMovementDetection()
{
	bCanDetectMovement = true;

	// 현재 위치 저장
	if (MainCharacter)
	{
		LastPlayerLocation = MainCharacter->GetActorLocation();
	}
	
	UE_LOG(LogTemp, Log, TEXT("🚨 감지 시작!"));
}


void ADocentV2::DetectPlayerMovement()
{
	FVector CurrentLocation = MainCharacter->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), CurrentLocation);
	
	if (DistanceToPlayer > MaxDetectionDistance)
	{
		UE_LOG(LogTemp, Verbose, TEXT("❌ 감지 거리 초과: %.1fcm"), DistanceToPlayer);
		return; // 너무 멀면 감지하지 않음
	}
	
	// 위치 변화량 감지
	float MovementDelta = FVector::Dist(LastPlayerLocation, CurrentLocation);

	if (MovementDelta > DetectRange) // 임계값 조정 가능
	{
		UE_LOG(LogTemp, Warning, TEXT(" 플레이어 움직임 감지됨!"));

		// 감지되면 실행할 함수
		
	}
	LastPlayerLocation = CurrentLocation;
}

void ADocentV2::StopMovementDetection()
{
	bCanDetectMovement = false;
	UE_LOG(LogTemp, Log, TEXT("🛑 감지 종료"));
}
