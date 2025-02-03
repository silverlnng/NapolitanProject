// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepSpawnerComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NapolitanProject/YJ/FootstepActor.h"

// Sets default values for this component's properties
UFootstepSpawnerComponent::UFootstepSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFootstepSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFootstepSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFootstepSpawnerComponent::SpawnFootsteps(const FVector& Destination)
{
	if (!GetOwner() || !FootstepActorClass) return;

	// 네비게이션 시스템 가져오기
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	if (!NavSystem) return;

	// 경로 찾기
	FNavLocation NavStart(GetOwner()->GetActorLocation());
	FNavLocation NavEnd(Destination);

	UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), NavStart.Location, NavEnd.Location);
	
	if (!NavPath || NavPath->PathPoints.Num() < 2) return;

	// 경로를 따라 발자국 생성
	FVector PreviousPoint = NavPath->PathPoints[0];

	for (int32 i = 1; i < NavPath->PathPoints.Num(); i++)
	{
		FVector CurrentPoint = NavPath->PathPoints[i];

		// 일정 거리마다 발자국 배치
		while (FVector::Dist(PreviousPoint, CurrentPoint) > StepDistance)
		{
			FVector Direction = (CurrentPoint - PreviousPoint).GetSafeNormal();
			PreviousPoint += Direction * StepDistance;

			// 발자국 스폰
			// 방향을 플레이어의 앞방향벡터로 하는게 자연스러움.
			float rot=GetOwner()->GetActorRotation().Yaw;
			GetWorld()->SpawnActor<AFootstepActor>(FootstepActorClass, PreviousPoint, FRotator(-90.f,rot,0));
			
			//GetWorld()->SpawnActor<AFootstepActor>(FootstepActorClass, PreviousPoint, rot);
		}
	}
}

