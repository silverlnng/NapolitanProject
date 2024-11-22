// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEvent_DownArt.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
ALevelEvent_DownArt::ALevelEvent_DownArt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());

	Box->SetRelativeLocation(FVector(0, -50.f, 30.f));

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(Box);
}

// Called when the game starts or when spawned
void ALevelEvent_DownArt::BeginPlay()
{
	Super::BeginPlay();

	LerpAlpha = 0.0f; // 초기 Lerp 알파값
	TargetZ = -110.f; // 목표 Z 위치
	MoveSpeed = 4.0f;

	Box->OnComponentBeginOverlap.AddDynamic(this, &ALevelEvent_DownArt::OnBeginOverlap);
}

// Called every frame
void ALevelEvent_DownArt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelEvent_DownArt::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약 부딪힌게 캐릭터일 경우 Frame은 아래로 내려감
	if(OtherActor)
	{
		auto* Target = Cast<ATestCharacter>(OtherActor);
		if(Target)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ScreamSound); //소리 내기
			// 시작 위치와 목표 위치 초기화
			StartLocation = Frame->GetRelativeLocation();
			TargetLocation = StartLocation;
			TargetLocation.Z = TargetZ; // 목표 Z 설정

			// 타이머를 사용하여 부드러운 이동 시작
			GetWorld()->GetTimerManager().SetTimer(LerpTimerHandle, this, &ALevelEvent_DownArt::MoveSmoothly, 0.01f, true);
		}
	}
}

void ALevelEvent_DownArt::MoveSmoothly()
{
	// LerpAlpha를 점진적으로 증가
	LerpAlpha += 0.01f * MoveSpeed;

	// Lerp를 통해 새로운 위치 계산
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpAlpha);

	// Frame의 위치를 업데이트
	Frame->SetRelativeLocation(NewLocation);

	// 이동 완료 체크
	if (LerpAlpha >= 1.0f)
	{
		// 타이머 정지
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
		GetWorldTimerManager().SetTimer(LerpTimerHandle, [this]()
			{
				MoveUp();
			}, 4.0f, false);
	}
}

void ALevelEvent_DownArt::MoveUp()
{
	// Lerp 초기화
	LerpAlpha = 0.0f;

	// 목표 위치와 시작 위치를 반대로 설정
	FVector CurrentLocation = Frame->GetRelativeLocation();
	StartLocation = CurrentLocation;
	TargetLocation = StartLocation;
	TargetLocation.Z = StartLocation.Z + 80.f; // 원래 위치로 이동

	// 타이머를 사용하여 부드러운 이동 시작
	GetWorld()->GetTimerManager().SetTimer(LerpTimerHandle, this, &ALevelEvent_DownArt::MoveSmoothlyUp, 0.01f, true);
}

void ALevelEvent_DownArt::MoveSmoothlyUp()
{
	// LerpAlpha를 점진적으로 증가
	LerpAlpha += 0.01f * MoveSpeed;

	// Lerp를 통해 새로운 위치 계산
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpAlpha);

	// Frame의 위치를 업데이트
	Frame->SetRelativeLocation(NewLocation);

	// 이동 완료 체크
	if (LerpAlpha >= 1.0f)
	{
		// 타이머 정지
		GetWorld()->GetTimerManager().ClearTimer(LerpTimerHandle);
	}
}



