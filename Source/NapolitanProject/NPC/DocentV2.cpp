// Fill out your copyright notice in the Description page of Project Settings.


#include "DocentV2.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/YJ/SoundControlActor.h"

// Sets default values
ADocentV2::ADocentV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
	// 몬스터 카메라 생성 및 초기화
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(GetMesh(),FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지
}

// Called when the game starts or when spawned
void ADocentV2::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	AIController = Cast<AAIController>(GetController());

	for (TActorIterator<ASoundControlActor> It(GetWorld(), ASoundControlActor::StaticClass()); It; ++It)
	{
		SoundControlActor = *It;
	}
	
	//StartTurnDetect();
}

// Called every frame
void ADocentV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//DrawDebugSphere(GetWorld(), GetActorLocation(), MaxDetectionDistance, 12, FColor::Blue, false, 0.1f);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Red, false, 0.1f);

	if (MainCharacter)
	{
		FVector CurrentLocation = MainCharacter->GetActorLocation();
	
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), CurrentLocation);
		
		if (!InMaxDetectionDistance && DistanceToPlayer <= MaxDetectionDistance)
		{
			InMaxDetectionDistance=true;
			// 음원변경
			if (SoundControlActor&&SoundControlActor->DocentBG)
			{
				SoundControlActor->BGSoundChange(SoundControlActor->DocentBG);
			}
			// 이때부터 StartTurnDetect(); 시작함
			StartTurnDetect();
			
		}
		if (InMaxDetectionDistance&&DistanceToPlayer>MaxDetectionDistance)
		{
			InMaxDetectionDistance=false;
			// 음원변경
			if (SoundControlActor&&SoundControlActor->LobbyRoom)
			{
				SoundControlActor->BGSoundChange(SoundControlActor->LobbyRoom);
			}
			StopAllTurnDetect();
		}
	}
	
	if (bCanDetectMovement && MainCharacter)
	{
		DetectPlayerMovement();
	}
	
	// 추적 상태일 때만 체크
	if (bHasDetectedPlayer && MainCharacter)
	{
		float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());

		if (Distance <= AttackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("💥 공격 범위 도달!"));

			// 공격 애니메이션 실행
			PlayAttackAnimation();

			// 이후 계속 공격하지 않도록 추적 상태 종료
			bHasDetectedPlayer = false;

			// AI 이동도 멈춤
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}




// ChoseRandomTimeTurnRightAnim 시작자체를
void ADocentV2::StartTurnDetect()
{
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay = 4.5f;
	}
	else
	{
		ChosenDelay = FMath::RandBool() ? 2.5f : 3.5f;
	}

	UE_LOG(LogTemp, Log, TEXT("뒤 돌아볼때까지 딜레이: %.1f초"), ChosenDelay);
	
	GetWorldTimerManager().SetTimer(
			  PlayTurnRightAnim,
			  this,
			  &ADocentV2::PlayTurnRightAnimation,
			  ChosenDelay,
			  false
		  );
	
	
}

void ADocentV2::PlayTurnRightAnimation()
{
	
	GetMesh()->PlayAnimation(TurnAroundMontage,false);
	
	float RandValue = FMath::FRand();
	float ChosenDelay = 0.f;

	if (RandValue <= 0.6f)
	{
		ChosenDelay = 8.0f;
	}
	else
	{
		ChosenDelay = 5.0f;
	}
	UE_LOG(LogTemp, Log, TEXT("앞으로 돌아볼때까지 딜레이: %.1f초"), ChosenDelay);
	
	// 0.5초 지난후 부터 플레이어의 움직임을 감지
	GetWorldTimerManager().SetTimer(
		  StartDetectionTimerHandle,
		  this,
		  &ADocentV2::StartMovementDetection,
		  1.5f,
		  false
	  );
	
	// 그리고 다시 원래대로 회전도 해야함 + 플레이어 감지 스탑

	GetWorld()->GetTimerManager().SetTimer(StopDetectionTimerHandle,[this]()
	{
		PlayTurnOriginAnimation();
		StopMovementDetection();
	},ChosenDelay,false);
}

void ADocentV2::PlayTurnOriginAnimation()
{
	if (TurnOriginMontage)
	{
		GetMesh()->PlayAnimation(TurnOriginMontage,false);
	}

	// 다시 시작
	StartTurnDetect();
}

void ADocentV2::StartMovementDetection()
{
	bCanDetectMovement = true;

	// 현재 위치 저장
	if (MainCharacter)
	{
		LastPlayerLocation = MainCharacter->GetActorLocation();
	}
	
	UE_LOG(LogTemp, Log, TEXT("🚨 감지 시작!"));
}


void ADocentV2::DetectPlayerMovement()
{
	FVector CurrentLocation = MainCharacter->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(GetActorLocation(), CurrentLocation);
	
	if (DistanceToPlayer > MaxDetectionDistance)
	{
		UE_LOG(LogTemp, Verbose, TEXT("❌ 감지 거리 초과: %.1fcm"), DistanceToPlayer);
		return; // 너무 멀면 감지하지 않음
	}
	
	// 위치 변화량 감지
	float MovementDelta = FVector::Dist(LastPlayerLocation, CurrentLocation);

	if (MovementDelta > DetectRange) // 임계값 조정 가능
	{
		UE_LOG(LogTemp, Warning, TEXT(" 플레이어 움직임 감지됨!"));

		// 감지되면 실행할 함수
		// 1. 감지 중지
		bCanDetectMovement = false;
		bHasDetectedPlayer = true;
		// 2. 타이머 취소
		if (GetWorldTimerManager().IsTimerActive(StopDetectionTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(StopDetectionTimerHandle);
			UE_LOG(LogTemp, Log, TEXT("🛑 StopDetection 타이머 취소됨"));
		}

		// 감지 소리 
		SoundControlActor->AudioComp2->Stop();
		
		if (DetectSound)
		{
			UGameplayStatics::PlaySound2D(this, DetectSound);
		}
		
		if (AIController && MainCharacter)
		{
			AIController->MoveToActor(MainCharacter);
			// 달리는 애니메이션 실행
			if (RunMontage)
			{
				GetMesh()->PlayAnimation(RunMontage,true);
			}
			UE_LOG(LogTemp, Log, TEXT("🏃 몬스터가 플레이어를 추적 시작"));
		}

		// 이후 반복 감지 막기 위해 종료
		return;
	}
	LastPlayerLocation = CurrentLocation;
}

void ADocentV2::StopMovementDetection()
{
	bCanDetectMovement = false;
	UE_LOG(LogTemp, Log, TEXT("🛑 감지 종료"));
}

void ADocentV2::PlayAttackAnimation()
{
	if (AttackMontage && !IsPlayingRootMotion()) // 중복 재생 방지
	{
		PlayAnimMontage(AttackMontage);
		UE_LOG(LogTemp, Log, TEXT("👊 공격 애니메이션 실행됨"));
	}

	MainCharacter->SetActorHiddenInGame(true);
	// 사망이벤트 만 발생시킴
	MainCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .
	SwitchToMonsterCamera();
	
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
	/*FTimerHandle SwitchCameraTimer;
	GetWorld()->GetTimerManager().SetTimer(SwitchCameraTimer,[this]()
	{
		
	},0.75f,false);*/
}

void ADocentV2::SwitchToMonsterCamera()
{
	if (TestPC && MonsterCamera)
	{
		// 카메라 전환
		TestPC->SetViewTargetWithBlend(this, 0.01f); // 0.5초 동안 부드럽게 전환
	}
}

void ADocentV2::StopAllTurnDetect()
{
	// 타이머 종료하고 
	
	if (GetWorldTimerManager().IsTimerActive(StartDetectionTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(StartDetectionTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("🛑 StartDetectionTimerHandle 타이머 취소됨"));
	}
	
	if (GetWorldTimerManager().IsTimerActive(PlayTurnRightAnim))
	{
		GetWorldTimerManager().ClearTimer(PlayTurnRightAnim);
		UE_LOG(LogTemp, Log, TEXT("🛑 PlayTurnRightAnim 타이머 취소됨"));
	}

	
	StopMovementDetection();
}
