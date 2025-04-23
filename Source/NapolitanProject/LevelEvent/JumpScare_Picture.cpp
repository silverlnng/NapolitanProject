// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpScare_Picture.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

class ATestCharacter;
class ATestPlayerController;
// Sets default values
AJumpScare_Picture::AJumpScare_Picture()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(GetMesh(),FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지

	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
	
	SceneCaptureComponent2D=CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AJumpScare_Picture::BeginPlay()
{
	Super::BeginPlay();
	// 앞으로 이동할 방향 저장
	InitialMoveDirection = GetActorForwardVector().GetSafeNormal();

	// 3초 후에 정지 및 추적 시작
	GetWorldTimerManager().SetTimer(StopMoveTimerHandle, this, &AJumpScare_Picture::StopInitialForwardMovement, 3.0f, false);

	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		TargetCharacter =TestPC->GetPawn<ATestCharacter>();
	}
}

// Called every frame
void AJumpScare_Picture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMovingForward)
	{
		AddMovementInput(InitialMoveDirection, MoveSpeed * DeltaTime);
	}
	else if (bIsChasing && TargetCharacter)
	{
		MoveTowardTarget(DeltaTime);
	}
}

// Called to bind functionality to input
void AJumpScare_Picture::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJumpScare_Picture::MovetoForward()
{
	// 앞으로 걷다가
	AddMovementInput(GetActorForwardVector());
	// 끝나면 그림밖으로 점프
	
	// 그리고 캐릭터 추격
}

void AJumpScare_Picture::StopInitialForwardMovement()
{
	bIsMovingForward = false;
	StartChasingTarget();
}

void AJumpScare_Picture::StartChasingTarget()
{
	bIsChasing = true;
}

void AJumpScare_Picture::MoveTowardTarget(float DeltaTime)
{
	if (!TargetCharacter) return;

	FVector Direction = (TargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction, MoveTowardTargetSpeed * DeltaTime);

	//캐릭터를 향해 오는 속도는 빠르게
	// 0.2초마다 거리 체크 (Tick 대신 Timer 사용)
	
	GetWorld()->GetTimerManager().SetTimer(ChaseCheckTimer, this, &AJumpScare_Picture::CheckAttackRange, 0.2f, true);
	
	//그리고 잡히면 점프스케어로 변경
}

void AJumpScare_Picture::CheckAttackRange()
{
	toPlayerDistance = FVector::Distance(this->GetActorLocation(), TargetCharacter->GetActorLocation());

	if (toPlayerDistance <= AttackRange)
	{
		// 움직임 멈추고, 
		bIsChasing = false;

		// 다른애니메이션( 공격애니메이션을) 실행
		
		// 점프스케어
		StartAttack();
	}
}

void AJumpScare_Picture::StartAttack()
{
	// 한번만 작동되도록 
	if (bAttack){return;}

	//공격소리로 바꾸기 

	if (AttackSound)
	{
		AudioComp->SetSound(AttackSound);
	}
	
	TargetCharacter->SetActorHiddenInGame(true);
	
	// 사망이벤트 만 발생시킴
	TargetCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .
	SwitchToMonsterCamera();
}

void AJumpScare_Picture::SwitchToMonsterCamera()
{
	if (TestPC && MonsterCamera)
	{
		// 카메라 전환
		TestPC->SetViewTargetWithBlend(this, 0.1f); // 0.5초 동안 부드럽게 전환

		// 카메라 흔들기 실행
		if (TestPC->PlayerCameraManager)
		{
			if (DeathCameraShakeClass)
			{
				TestPC->PlayerCameraManager->StartCameraShake(DeathCameraShakeClass);
				
			}
		}
	}
}




