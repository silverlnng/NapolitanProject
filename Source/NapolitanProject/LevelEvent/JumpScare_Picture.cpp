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

	AttachedStaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachedStaticMeshComp"));
	AttachedStaticMeshComponent->SetupAttachment(GetMesh(),FName(TEXT("hand_rSocket"))); // 루트에 부착
	PaintingCanvasStaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanvasStaticMeshComp"));
	PaintingCanvasStaticMeshComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void AJumpScare_Picture::BeginPlay()
{
	Super::BeginPlay();
	// 앞으로 이동할 방향 저장
	//InitialMoveDirection = GetActorForwardVector().GetSafeNormal();


	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		TargetCharacter =TestPC->GetPawn<ATestCharacter>();
	}

	AnimInstance = GetMesh()->GetAnimInstance();

	
	AttachedStaticMeshComponent->SetHiddenInGame(true);
	PaintingCanvasStaticMeshComponent->SetHiddenInGame(true);
	
}

// Called every frame
void AJumpScare_Picture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (bIsMovingForward)
	{
		AddMovementInput(InitialMoveDirection, MoveSpeed * DeltaTime);
	}
	else if (bIsChasing && TargetCharacter)
	{
		MoveTowardTarget(DeltaTime);
	}*/
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
}

void AJumpScare_Picture::StopInitialForwardMovement()
{
	bIsMovingForward = false;
	//StartChasingTarget();

	// 멈추고 서있는 애니메이션 실행.
	
	// 거리체크를 시작
	//GetWorld()->GetTimerManager().SetTimer(ChaseCheckTimer, this, &AJumpScare_Picture::CheckAttackRange, 0.2f, true);
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
	
	//GetWorld()->GetTimerManager().SetTimer(ChaseCheckTimer, this, &AJumpScare_Picture::CheckAttackRange, 0.2f, true);
	
	//그리고 잡히면 점프스케어로 변경
}

void AJumpScare_Picture::CheckAttackRange()
{
	toPlayerDistance = FVector::Distance(this->GetActorLocation(), TargetCharacter->GetActorLocation());

	if (toPlayerDistance <= AttackRange)
	{
		if (bAttackRangeIn)
		{
			return;
		}
		bAttackRangeIn = true; // 한번만 작동시키도록
		// 캐릭터 쪽으로 움직이면서 
		StartChasingTarget();
		// 다른애니메이션( 공격애니메이션을) 실행 ==> 오직한번만실행.
		PlayRunAnimMontage();
		// 점프스케어
		//StartAttack();
	}
}

void AJumpScare_Picture::StartAttack()
{
	// 한번만 작동되도록 
	if (bAttack){return;}


	// 콜리전 세팅을 바꾸기 (그림에서 통과되도록)
	
	//공격소리로 바꾸기 

	if (AttackSound)
	{
		AudioComp->SetSound(AttackSound);
	}
	
	TargetCharacter->SetActorHiddenInGame(true);
	
	// 사망이벤트 만 발생시킴
	TargetCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .

	// 시간지연을 주고 
	FTimerHandle delayTimer;

	GetWorld()->GetTimerManager().SetTimer(delayTimer,[this]()
	{
		SwitchToMonsterCamera();
	},SwitchToMonsterCameraDelay,false);
	
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

void AJumpScare_Picture::PlayBasicAnimMontage()
{
	
	//PlayAnimMontage(basicAnimMontage);
	
	if (AnimInstance&&basicAnimMontage)
	{
		AnimInstance->Montage_Play(basicAnimMontage);
	}
	// 앞으로 움직임 시작
	//bIsMovingForward=true;
	
	//GetWorldTimerManager().SetTimer(StopMoveTimerHandle, this, &AJumpScare_Picture::StopInitialForwardMovement, InitialMoveDuration, false);
}

void AJumpScare_Picture::PlaySittingAnimMontage()
{
	if (AnimInstance&&SitAnimMontage)
	{
		AnimInstance->Montage_Play(SitAnimMontage);
	}

	FTimerHandle NextAnimTimer1;
	GetWorld()->GetTimerManager().SetTimer(NextAnimTimer1,[this]()
	{
		PlayPaintAnimMontage();
	},5.5f,false);
}

void AJumpScare_Picture::PlayPaintAnimMontage()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayPaintAnimMontage");
	
	// 시간 지연을 주고 회전
	FRotator CurrentRotation = GetActorRotation();

	// Yaw 값을 -90도 감소 (왼쪽으로 회전) 
	CurrentRotation.Yaw -= 90.0f;

	// 새로운 회전값 설정
	SetActorRotation(CurrentRotation);

	// 붓 아이템을 보이도록 하기 
	AttachedStaticMeshComponent->SetHiddenInGame(false);

	// 그림 메쉬 보이도록 하기 

	
	BlueprintImplementableEventPlayPaintAnim();

	// 페인트 애니가 끝나고 다시 회전 시키기

	
	
}

void AJumpScare_Picture::PaintAnimEnd()
{
	
}

void AJumpScare_Picture::PlayRunAnimMontage()
{
	if (!AnimInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AnimInstance is null");
	}
	
	if (AnimInstance&&attackAnimMontage)
	{
		AnimInstance->Montage_Play(attackAnimMontage);
		AnimInstance->Montage_JumpToSection(FName("Run"), attackAnimMontage);
	}
}

void AJumpScare_Picture::PlayJumpAttackAnimMontage()
{
	if (AnimInstance&&attackAnimMontage)
	{
		GetMesh()->PlayAnimation(JumpAnimMontage, true);
		//AnimInstance->Montage_Play(JumpAnimMontage);
		//AnimInstance->Montage_JumpToSection(FName("Jump"), attackAnimMontage);
		// attackAnimMontage를 달리기+ 점프공격 으로 바꾸기 !!! 
	}
}




