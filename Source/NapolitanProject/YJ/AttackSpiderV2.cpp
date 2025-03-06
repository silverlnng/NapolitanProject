// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpiderV2.h"

#include "AIController.h"
#include "AttackSpiderAIController.h"
#include "AttackSpider_AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "Perception/AISenseConfig_Hearing.h"

// Sets default values
AAttackSpiderV2::AAttackSpiderV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
	
}

// Called when the game starts or when spawned
void AAttackSpiderV2::BeginPlay()
{
	Super::BeginPlay();

	Anim=Cast<UAttackSpider_AnimInstance>(GetMesh()->GetAnimInstance());
	
	AIController = Cast<AAttackSpiderAIController>(GetController());
	if (AIController)
	{
		//AIController->SetPerceptionComponent(*AIPerception);
	}
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	if (SplineActor)
	{
		CurrentSpline=Cast<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	}
	if (CurrentSpline==nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentSpline Null")));
	}
	
	StartMoving();


}

// Called every frame
void AAttackSpiderV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving && CurrentSpline)
	{
		MoveAlongSpline(DeltaTime);
	}
}

void AAttackSpiderV2::MoveAlongSpline(float DeltaTime)
{
	if (!CurrentSpline) return;

	// 이동 거리 증가
	DistanceAlongSpline += MoveSpeed * DeltaTime;
	
	float SplineLength = CurrentSpline->GetSplineLength();

	// 🎯 Spline 끝에 도달하면 처음으로 되돌리기
	if (DistanceAlongSpline >= SplineLength)
	{
		DistanceAlongSpline = 0.0f; // 🎯 다시 시작점으로 이동
	}

	// 새로운 위치 및 회전 계산
	FVector NewLocation = CurrentSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator NewRotation = CurrentSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

	// 몬스터를 거꾸로 매달기 위해 추가 회전 적용
	NewRotation.Pitch += 180.0f; 
	
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AAttackSpiderV2::StartMoving()
{
	if (CurrentSpline)
	{
		DistanceAlongSpline = 0.0f;
		bIsMoving = true;
	}
}

void AAttackSpiderV2::AttackPlayer()
{
	
	// 공격시작
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("공격시작")));
}

void AAttackSpiderV2::DetectAndDrop()
{
	//움직임을 멈추고
	bIsMoving=false;
	//플레이어를 보는 방향으로 회전
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), MainCharacter->GetActorLocation());
	LookAtRotation.Pitch=0;
	SetActorRotation(LookAtRotation);
}

void AAttackSpiderV2::StartChasing()
{
	// 플레이어 향해 추적
	AIController->MoveToActor(MainCharacter, 5.0f);

	// 0.2초마다 거리 체크 (Tick 대신 Timer 사용)
	GetWorld()->GetTimerManager().SetTimer(ChaseCheckTimer, this, &AAttackSpiderV2::CheckAttackRange, 0.2f, true);
	
}
void AAttackSpiderV2::StartAttack()
{
	// 사망이벤트 만 발생시킴
	
	//카메라 쉐이크 .
}

void AAttackSpiderV2::CheckAttackRange()
{
	Distance = FVector::Distance(this->GetActorLocation(), MainCharacter->GetActorLocation());

	if (Distance <= AttackRange)
	{
		SetAIState(EAttackSpiderV2State::Attack);
	}
	else
	{
		SetAIState(EAttackSpiderV2State::Chase);
	}
	
}


void AAttackSpiderV2::SetAIState(EAttackSpiderV2State NewState)
{
	FString message = UEnum::GetValueAsString(NewState);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	
	CurrentState = NewState;
	if (Anim)
	{
		Anim->CurrentState=NewState;
	}
	switch (CurrentState)
	{
	case EAttackSpiderV2State::Move:
		StartMoving();
		break;
	case EAttackSpiderV2State::Drop:
		DetectAndDrop();
		break;
	case EAttackSpiderV2State::Chase:
		StartChasing();
		break;
	case EAttackSpiderV2State::Attack:
		StartAttack();
		break;
	}
}



