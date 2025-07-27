// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpider.h"

#include "AIController.h"
#include "AttackSpider_AnimInstance.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAttackSpider::AAttackSpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
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

	Anim=Cast<UAttackSpider_AnimInstance>(GetMesh()->GetAnimInstance());

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MoveTarget"), TargetPoints);

	MoveToNextLocation();
	StartIdle();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&AAttackSpider::OnSeePawn);

	//장애물 회피
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f;
}

// Called every frame
void AAttackSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAttackSpider::SetAIState(EAttackSpiderState NewState)
{
	
	FString message = UEnum::GetValueAsString(NewState);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	
	CurrentState = NewState;
	if (Anim)
	{
		//Anim->CurrentState=NewState;
	}
	switch (CurrentState)
	{
	case EAttackSpiderState::Idle:
		StartIdle();
		break;
	case EAttackSpiderState::MoveToNextPoint:
		MoveToNextLocation();
		break;
	case EAttackSpiderState::Chase:
		StartChasing();
		break;
	case EAttackSpiderState::Attack:
		StartAttack();
		break;
	}
}

void AAttackSpider::StartIdle()
{
	// 10초 후 MoveToNextLocation 실행
	GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &AAttackSpider::MoveToNextLocation, 5.0f, false);
}

void AAttackSpider::MoveToNextLocation()
{
	if (TargetPoints.Num() == 0) return;

	int32 RandomIndex = FMath::RandRange(0, TargetPoints.Num() - 1);
	FVector TargetLocation = TargetPoints[RandomIndex]->GetActorLocation();
    
	SetActorLocation(TargetLocation);
}

void AAttackSpider::StartChasing()
{
	if (!MainCharacter) return;

	// 이걸 animation 몽타주 쪽에서 실행하기 
	AIController->MoveToActor(MainCharacter, 5.0f); // 플레이어를 향해 이동

	// 0.2초마다 거리 체크 (Tick 대신 Timer 사용)
	GetWorld()->GetTimerManager().SetTimer(ChaseCheckTimer, this, &AAttackSpider::CheckAttackRange, 0.2f, true);

	// 3초 동안 플레이어를 못 보면 Idle로 변경
	GetWorld()->GetTimerManager().SetTimer(LostSightTimerHandle, this, &AAttackSpider::HandleLostSight, 3.0f, false);
}

void AAttackSpider::StartAttack()
{
	//잠깐 이동을 멈추고
	GetCharacterMovement()->StopMovementImmediately();
	
	// 공격 중에는 감지를 비활성화해서 OnSeePawn()이 실행되지 않도록 함
	PawnSensingComp->SetSensingUpdatesEnabled(false);
	// 공격 애니메이션 실행
	PlayAnimMontage(AttackMontage);
}

void AAttackSpider::OnSeePawn(APawn *OtherPawn)
{
	/*if (OtherPawn)
	{
		FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	}*/
	
	ATestCharacter* Player = Cast<ATestCharacter>(OtherPawn);
	if (Player)
	{
		// Idle 상태의 타이머가 돌고 있다면 취소
		GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);

		// 시야 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(LostSightTimerHandle); 
		
		// 추적 상태로 전환
		SetAIState(EAttackSpiderState::Chase);
	}
}

void AAttackSpider::HandleLostSight()
{
	// 아직도 CouldSeePawn이 false면 Idle로 변경
	if (!PawnSensingComp->CouldSeePawn(MainCharacter))
	{
		SetAIState(EAttackSpiderState::Idle);
	}
}


void AAttackSpider::CheckAttackRange()
{
	if (!MainCharacter) return;
	
	
	float Distance = FVector::Distance(this->GetActorLocation(), MainCharacter->GetActorLocation());

	if (Distance <= AttackRange) // 공격 범위 안이면
	{
		// 타이머 정지 후 공격 상태로 전환
		GetWorld()->GetTimerManager().ClearTimer(ChaseCheckTimer);
		
		SetAIState(EAttackSpiderState::Attack);
	}
	
}

void AAttackSpider::MoveToActor()
{
	// 이걸 animation 몽타주 쪽에서 실행하기 
	AIController->MoveToActor(MainCharacter, 5.0f);
}


// AttackMontage 에서 끝에서 실행 .
void AAttackSpider::CheckAfterAttack()
{
	if (!MainCharacter) return;
	
	SetAIState(EAttackSpiderState::Chase);
	// 공격 중에는 감지를 비활성화해서 OnSeePawn()이 실행되지 않도록 함
	PawnSensingComp->SetSensingUpdatesEnabled(true);
	
	float Distance = FVector::Distance(this->GetActorLocation(), MainCharacter->GetActorLocation());

	if (Distance <= AttackRange)
	{
		// 다시 공격
		StartAttack();
	}
	else
	{
		PawnSensingComp->SetSensingUpdatesEnabled(true);
		// 플레이어가 멀어졌으면 추적
		SetAIState(EAttackSpiderState::Chase);
	}
}


