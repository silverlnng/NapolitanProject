// Fill out your copyright notice in the Description page of Project Settings.


#include "EyesManager.h"

#include "OriginEye.h"
#include "Components/BoxComponent.h"

// Sets default values
AEyesManager::AEyesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
}

// Called when the game starts or when spawned
void AEyesManager::BeginPlay()
{
	Super::BeginPlay();

	// TriggerBox 영역 안에 있는 OriginEye들만 수집
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors, AOriginEye::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (AOriginEye* Eye = Cast<AOriginEye>(Actor))
		{
			OriginEyes.Add(Eye);
			Eye->SetEyeVisible(false); // 초기엔 숨김
		}
	}

	TimeElapsed = 0.0f;
	
}

// Called every frame
void AEyesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	UpdateEyeVisibility();

	if (TimeElapsed >= TotalTime)
	{
		//SetActorTickEnabled(false); // 더 이상 Tick 안 돌리게
		//눈알 보이는 거 종료

		//제한시간이 끝났을 경우 사망 이벤트 발생
		if(TimeElapsed >= EndTime)
		{
			OnAllEyesRevealed(); // 시간 종료 → 이벤트 실행
		}
	}

}

void AEyesManager::UpdateEyeVisibility()
{
	/*float RevealRatio = FMath::Clamp(TimeElapsed / TotalTime, 0.0f, 1.0f);
	int32 RevealCount = FMath::RoundToInt(OriginEyes.Num() * RevealRatio);

	for (int32 i = 0; i < OriginEyes.Num(); ++i)
	{
		bool bShouldBeVisible = i < RevealCount;
		if (OriginEyes[i])
		{
			OriginEyes[i]->SetEyeVisible(bShouldBeVisible);
		}
	}*/

	//1분안에 모든 눈알이 보이도록 코드 조정
	float RevealRatio = FMath::Clamp(TimeElapsed / TotalTime, 0.0f, 1.0f);
	int32 RevealCount = FMath::CeilToInt(OriginEyes.Num() * RevealRatio);

	for (int32 i = 0; i < OriginEyes.Num(); ++i)
	{
		bool bShouldBeVisible = i < RevealCount;
		if (OriginEyes[i])
		{
			OriginEyes[i]->SetEyeVisible(bShouldBeVisible);
		}
	}
}

void AEyesManager::OnAllEyesRevealed()
{
	UE_LOG(LogTemp, Warning, TEXT("모든 OriginEye가 나타났습니다!"));
	// 제한시간이 지났음에도 문제를 풀지 못했다면 사망
}

