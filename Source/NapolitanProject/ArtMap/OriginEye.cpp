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
	EyeSphere->SetupAttachment(RootComponent);	

	//애로우 컴포넌트
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(EyeSphere);

	EyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>EyeMeshComp(TEXT("/Script/Engine.StaticMesh'/Game/YJ/BP/SecurityMission/Eye/16_eye.16_eye'"));
	if(EyeMeshComp.Succeeded())
	{
		EyeMesh->SetStaticMesh(EyeMeshComp.Object);
		EyeMesh->SetupAttachment(Arrow); //애로우 컴포넌트에 붙이기
	}

	
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
	
}

// Called every frame
void AOriginEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MainCharacter)
	{
		//플레이어와의 방향 계산
		FVector PlayerLocation = MainCharacter->GetActorLocation();
		FVector ArrowLocation = GetActorLocation();

		//두 위치 간의 벡터 계산 후 회전 값으로 변환
		FRotator LookAtRotation = (PlayerLocation - ArrowLocation).Rotation();

		FRotator SmoothRotation = FMath::RInterpTo(
			Arrow->GetComponentRotation(), // 현재 회전
			LookAtRotation,                // 목표 회전
			DeltaTime,                     // 델타 시간
			5.0f                           // 회전 속도
		);

		//애로우 컴포넌트 회전 업데이트
		Arrow->SetWorldRotation(SmoothRotation);
	}
	

}

