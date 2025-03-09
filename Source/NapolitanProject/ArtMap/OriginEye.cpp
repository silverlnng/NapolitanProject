// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginEye.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

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
	}

	bShouldLookAtPlayer = false;
	RandomLookInterval = 1.5f;
	TimeSinceLastRandomLook = 0.0f;
	RandomSpeed = FMath::RandRange(150.0f, 250.0f);
}

// Called every frame
void AOriginEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainCharacter) return;

	FVector PlayerLocation = MainCharacter->GetActorLocation();
	FVector EyeLocation = GetActorLocation();
	float DistanceToPlayer = FVector::Dist(PlayerLocation, EyeLocation);

	if (DistanceToPlayer <= 1200.0f)
	{
		bShouldLookAtPlayer = true;
	}
	else
	{
		TimeSinceLastRandomLook += DeltaTime;
		if (TimeSinceLastRandomLook >= RandomLookInterval)
		{
			RandomLookDirection = FRotator(FMath::RandRange(0.0f, 360.0f), FMath::RandRange(0.0f, 360.0f), FMath::RandRange(0.0f, 360.0f));
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

