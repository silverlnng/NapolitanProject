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
	bIsOnCeiling=false;
	CheckCeilingAndGround();  
}

// Called every frame
void AAttackSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckCeilingAndGround();  // 천장 & 지상 감지
	MoveAI();                 // AI 이동 처리
	CheckForPlayer();  
}


void AAttackSpider::CheckCeilingAndGround()
{
	FVector Start = GetActorLocation();
    
	// 1. 천장 감지 (위로 라인 트레이스)
	FVector CeilingEnd = Start + FVector(0, 0, TraceDistance);
	FHitResult CeilingHit;
    
	bool bHitCeiling=GetWorld()->LineTraceSingleByChannel(CeilingHit, Start, CeilingEnd, ECC_Visibility);
	
	// 🔹 디버그 라인 (천장 탐색용)
	DrawDebugLine(
		GetWorld(),
		Start,
		CeilingEnd,
		bHitCeiling ? FColor::Green : FColor::Red, // 천장이 감지되면 초록, 아니면 빨강
		false,
		0.1f,
		0,
		2.0f
	);
	
	if (bHitCeiling && bIsOnCeiling)
	{
		SwitchToCeilingMode();  // 천장 이동 모드로 전환
		return;
	}
	

	// 2. 지상 감지 (아래로 라인 트레이스)
	FVector GroundEnd = Start - FVector(0, 0, TraceDistance);
	FHitResult GroundHit;

	bool bHitGround =GetWorld()->LineTraceSingleByChannel(GroundHit, Start, GroundEnd, ECC_Visibility);

	DrawDebugLine(
		GetWorld(),
		Start,
		GroundEnd,
		bHitGround ? FColor::Blue : FColor::Red, // 지상이 감지되면 파랑, 아니면 빨강
		false,
		0.1f,
		0,
		2.0f
	);
	
	if (bHitGround && !bIsOnCeiling)
	{
		SwitchToGroundMode();  // 지상 이동 모드로 전환
	}
}

void AAttackSpider::SwitchToCeilingMode() //한번만 실행 . 
{
	// bIsOnCeiling = true;
	if (bIsOnCeiling){return;}
	// 중력 반전
	GetCharacterMovement()->GravityScale = -1.0f;

	// 천장의 표면 법선(Normal) 방향으로 NPC 회전
	FRotator CeilingRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + FVector(0, 0, 1));
	SetActorRotation(FRotator(180.0f, GetActorRotation().Yaw, 0));
	
	bIsOnCeiling=true;
	
	UE_LOG(LogTemp, Warning, TEXT("%s Switched to Ceiling Mode!"),*CALLINFO);
}

void AAttackSpider::SwitchToGroundMode()
{
	//

	// 중력 원래대로 복구
	GetCharacterMovement()->GravityScale = 1.0f;

	// 지면 방향으로 NPC 회전
	SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw, 0));

	bIsOnCeiling = false;
	
	UE_LOG(LogTemp, Warning, TEXT("%s Switched to Ground Mode!"),*CALLINFO);
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

		// 천장 모드일 때 높이 보정
		if (bIsOnCeiling)
		{
			Destination.Z += 400.0f;  // 천장 높이로 조정
		}

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
	
	// 감지 범위 내에 있으면 지상으로 내려감
	if (bIsOnCeiling && DistanceToPlayer <= PlayerDetectionRange)
	{
		AttackPlayer();
	}
	
	if (DistanceToPlayer > PlayerDetectionRange) // 멀어지면 다시 천장으로
	{
		SwitchToCeilingMode();
	}
	
}

void AAttackSpider::AttackPlayer()
{
	// 지상 이동 모드로 전환
	SwitchToGroundMode();

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

void AAttackSpider::ReturnToCeiling()
{
	SwitchToCeilingMode();
	bIsOnCeiling = true;
	
	/*if (AIController)
	{
		FVector CeilingTarget = GetActorLocation() + FVector(0, 0, 300.0f);
		AIController->MoveToLocation(CeilingTarget);
	}*/

	UE_LOG(LogTemp, Warning, TEXT("Returning to Ceiling!"));
}