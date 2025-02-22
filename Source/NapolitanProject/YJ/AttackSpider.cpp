// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpider.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NapolitanProject/NapolitanProject.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AAttackSpider::AAttackSpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// 천장 여러곳에 장소두고 랜덤으로 돌아다니기 . 5초동안 있다가 다른 천장장소로 이동 (Idle)

// 그러다가 캐릭터 발견하면 추적- 가까우면 공격 -

// 추적하다가 어느정도 멀어지면 다시 idle으로 돌아가기 


// Called when the game starts or when spawned
void AAttackSpider::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}

}

// Called every frame
void AAttackSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckCeilingAndGround();  // 천장 & 지상 감지
	MoveAI();                 // AI 이동 처리
	CheckForPlayer();  
}



void AAttackSpider::MoveAI()
{
	if (!AIController) return;

	// 이동할 랜덤 위치 가져오기
	// 이동 가능한 랜덤 위치 찾기
	FNavLocation RandomNavLocation;
	if (NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 5000.0f, RandomNavLocation))
	{
		FVector Destination = RandomNavLocation.Location;
		
		AIController->MoveToLocation(Destination);
	}
}

void AAttackSpider::CheckForPlayer()
{
	
	if (!MainCharacter) return;

	// NPC와 플레이어 사이의 거리 계산
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());

	// 플레이어와 NPC 사이에 디버그 라인 표시
	// 감지 범위 안에 있는 경우 초록색, 아니면 빨간색
	FColor DebugColor = (DistanceToPlayer <= PlayerDetectionRange) ? FColor::Emerald : FColor::White;
	
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),                   // 시작점 (NPC 위치)
		MainCharacter->GetActorLocation(),       // 끝점 (플레이어 위치)
		DebugColor,                          // 선 색상
		false,                                // 지속 여부 (false면 몇 초 후 사라짐)
		0.1f,                                 // 지속 시간
		0,                                    // 두께 옵션
		3.0f                                  // 선 두께
	);
	
}

void AAttackSpider::AttackPlayer()
{
	
	// 플레이어 방향으로 이동
	if (MainCharacter)
	{
		if (AIController)
		{
			AIController->MoveToActor(MainCharacter, 50.0f);  // 플레이어 근처까지 이동
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("Attacking Player!"));
	
	//FTimerHandle TimerHandle_Return;
	// 5초 후에 다시 천장으로 복귀
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_Return, this, &AAttackSpider::ReturnToCeiling, 5.0f, false);
}

