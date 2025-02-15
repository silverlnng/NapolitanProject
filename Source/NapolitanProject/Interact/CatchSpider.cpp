// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchSpider.h"

#include "AIController.h"
#include "AITypes.h"
#include "CatchSpider_AnimInstance.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACatchSpider::ACatchSpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACatchSpider::BeginPlay()
{
	Super::BeginPlay();
	EnemyAI = Cast<AAIController>(this->GetController());
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	Anim=Cast<UCatchSpider_AnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ACatchSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Died){return;}
	
	FVector Destination = PatrolPoint;

	auto* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Destination);
	MoveRequest.SetAcceptanceRadius(50);

	FPathFindingQuery Query;
	EnemyAI->BuildPathfindingQuery(MoveRequest, Query);
	FPathFindingResult PathResult = NavSystem->FindPathSync(Query);

	// 경로 탐색 성공 여부 확인
	if (PathResult.Result == ENavigationQueryResult::Success)
	{
		EPathFollowingRequestResult::Type MoveResult = EnemyAI->MoveToLocation(Destination);

		// 도착했거나 실패하면 새 패트롤 지점 설정
		if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal ||
			MoveResult == EPathFollowingRequestResult::Failed)
		{
			SetPatrolPoint(GetActorLocation(), PatrolPointRadius, PatrolPoint);
		}
	}
	else
	{
		// 경로 탐색 실패 시 다른 랜덤 위치를 설정
		SetPatrolPoint(GetActorLocation(), PatrolPointRadius, PatrolPoint);
	}
}



bool ACatchSpider::SetPatrolPoint(FVector origin, float radius, FVector& dest)
{
	// 길위의 랜덤한 위치를 정하고싶다.
	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool isSuccess = ns->GetRandomReachablePointInRadius(origin, radius, loc);
	// 만약 성공했다면
	if ( isSuccess )
	{
		// 그 위치를 dest에 적용하고싶다.
		dest = loc.Location;
	}
	return isSuccess;
}

void ACatchSpider::Damaged()
{
	// 피격 애니메이션 실행
	Anim->PlayDamagedMontage();
}

void ACatchSpider::Die()
{
	Died=true;
	// 죽는 애니메이션 실행
	Anim->bDied=true;
	// 피격 나이아가라 실행
	
	// 아이템 인벤토리 창에 담아지기
	FTimerHandle DieTimer;
	GetWorld()->GetTimerManager().SetTimer(DieTimer,[this]()
	{
		Destroy();
	},3.0f,false);
	// 3초뒤에 사라지도록 하기
}

