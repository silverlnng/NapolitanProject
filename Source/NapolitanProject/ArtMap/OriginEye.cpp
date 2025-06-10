// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginEye.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/DeadEndingWidget.h"

// Sets default values
AOriginEye::AOriginEye()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//콜리전
	EyeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EyeSphere"));
	SetRootComponent(EyeSphere);

	//애로우 컴포넌트
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(EyeSphere);

	EyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeMesh"));
	EyeMesh->SetupAttachment(Arrow); //애로우 컴포넌트에 붙이기
	
}

// Called when the game starts or when spawned
void AOriginEye::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	}
	

	bShouldLookAtPlayer = false;
	RandomLookInterval = 1.5f;
	TimeSinceLastRandomLook = 0.0f;
	RandomSpeed = FMath::RandRange(180.0f, 300.0f);
	
	
}

// Called every frame
void AOriginEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainCharacter) return;

	FVector PlayerLocation = MainCharacter->GetActorLocation();
	FVector EyeLocation = GetActorLocation();
	float DistanceToPlayer = FVector::Dist(PlayerLocation, EyeLocation);

	if (DistanceToPlayer <=1100.0f)
	{
		bShouldLookAtPlayer = true;
	}
	else
	{
		TimeSinceLastRandomLook += DeltaTime;
		if (TimeSinceLastRandomLook >= RandomLookInterval)
		{
			RandomLookDirection = FRotator(
				FMath::FRandRange(-60.0f, 60.0f),   // Pitch: -60 ~ 60도 (고개 위아래)
			FMath::FRandRange(0.0f, 270.0f), 
				0.0f);
			//RandomSpeed = FMath::RandRange(15.0f, 20.0f);
			TimeSinceLastRandomLook = 0.0f;
		}
		bShouldLookAtPlayer = false;
	}

	FRotator TargetRotation = bShouldLookAtPlayer ? (PlayerLocation - EyeLocation).Rotation() : RandomLookDirection;
    
	FRotator SmoothRotation = FMath::RInterpTo(
		Arrow->GetComponentRotation(),
		TargetRotation,
		DeltaTime,
		2.0f
	);

	Arrow->SetWorldRotation(SmoothRotation);

}

void AOriginEye::SetEyeVisible(bool bVisible)
{
	if (EyeMesh)
	{
		EyeMesh->SetVisibility(bVisible, true);
		EyeMesh->SetHiddenInGame(!bVisible, true);
	}
}

void AOriginEye::UpdateChasing(float DeltaTime)
{
	//제한 시간 내에 자물쇠를 못풀었을 시 플레이어를 향해 달려옴

	if (!MainCharacter) return;

	FVector PlayerLocation = MainCharacter->GetActorLocation();
    
	// 플레이어 주변 구형으로 분산
	float Radius = 300.0f; // 플레이어 주변 반지름
	FVector RandomDirection = FMath::VRand(); // 완전 랜덤 방향
	FVector TargetLocation = PlayerLocation + (RandomDirection * Radius);

	FVector EyeLocation = GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - EyeLocation + FVector(0.f, 0.f, 50.f)).GetSafeNormal();

	CurrentVelocity += DirectionToTarget * ChaseAcceleration * DeltaTime;
    
	if (CurrentVelocity.Size() > ChaseSpeed)
	{
		CurrentVelocity = CurrentVelocity.GetSafeNormal() * ChaseSpeed;
	}

	FVector NewLocation = EyeLocation + CurrentVelocity * DeltaTime;
	SetActorLocation(NewLocation);

	Arrow->SetWorldRotation(DirectionToTarget.Rotation());

	// 플레이어와 충돌 거리 체크 (게임오버 조건)
	float DistanceToPlayer = FVector::Dist(PlayerLocation, EyeLocation);
	if (DistanceToPlayer < 200.0f) // 1미터 이내로 접근하면
	{
		FTimerHandle UITimer2;
		GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
		{
			if (PlayerHUD)
			{
				PlayerHUD->PlayDeadVignetteEffect();
			}
		},0.2f,false);
	
		//시간 지연 주고 사망 UI 나오도록
		FTimerHandle UITimer;
		GetWorldTimerManager().SetTimer(UITimer, [this]()
		{
			MainCharacter->SetPlayerState(EPlayerState::UI);

			if(PlayerHUD && PlayerHUD->DeadEndingWidgetUI)
			{
				PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
				PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
			}
		},0.6f,false); //사망
	}
}

