// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpiderV2.h"

#include "AIController.h"
#include "AttackSpiderAIController.h"
#include "AttackSpider_AnimInstance.h"
#include "DeadEndingWidget.h"
#include "EngineUtils.h"
#include "SoundControlActor.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"


// Sets default values
AAttackSpiderV2::AAttackSpiderV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
	// 몬스터 카메라 생성 및 초기화
	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(RootComponent); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지

	AudioComp2 =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp2"));
	AudioComp2->SetupAttachment(GetCapsuleComponent());
	
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
	
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();	
	if (SplineActor)
	{
		CurrentSpline=Cast<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	}
	if (CurrentSpline==nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentSpline Null")));
	}
	
	StartMoving();

	for (TActorIterator<ASoundControlActor> It(GetWorld(), ASoundControlActor::StaticClass()); It; ++It)
	{
		SoundControlActor = *It;
		SoundControlActor->IsSecondFloor=true; // 로비에서는 나오는 다른소리안들리도록 제어
	}
	
}

// Called every frame
void AAttackSpiderV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving && CurrentSpline)
	{
		MoveAlongSpline(DeltaTime);
	}

	//계속해서 거리체크를 하면서 가까워지면 배경음을 감소시키기
	//멀어지면 증가시키고
	SoundControl();
	
}

void AAttackSpiderV2::MoveAlongSpline(float DeltaTime)
{
	if (!CurrentSpline) return;

	// 이동 거리 증가
	DistanceAlongSpline += SplineMoveSpeed * DeltaTime;
	
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
	NewRotation.Yaw += 180.0f; 
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
	// 소리
	if (DetectSound)
	{
		AudioComp->SetSound(DetectSound);
	}
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
	// 한번만 작동되도록 
	if (bAttack){return;}

	//공격소리로 바꾸기 

	if (AttackSound)
	{
		AudioComp->SetSound(AttackSound);
	}
	
	MainCharacter->SetActorHiddenInGame(true);
	// 플레이어이의 총안보이도록 하기
	TArray<AActor*> AttachedActors;
	MainCharacter->GetAttachedActors(AttachedActors);
	for (AActor* ChildActor : AttachedActors)
	{
		if (ChildActor)
		{
			ChildActor->SetActorHiddenInGame(true);
		}
	}
	// 사망이벤트 만 발생시킴
	MainCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .
	SwitchToMonsterCamera();
	
	// 사망 비네트 효과 
	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD )
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
		}
	},2.5f,false);
	//시간지연 주고 사망 UI 나오도록 
	FTimerHandle UITimer;

	GetWorld()->GetTimerManager().SetTimer(UITimer,[this]()
	{
		MainCharacter->SetPlayerState(EPlayerState::UI);
		
		if (PlayerHUD &&PlayerHUD->DeadEndingWidgetUI)
		{
			PlayerHUD->DeadEndingWidgetUI->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->DeadEndingWidgetUI->SetTextBlock_description(description);
			//FString name= FString(TEXT("<Red_Big>거미 에게</>"));
			//PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
			//PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();
			//PlayerHUD->PlayDeadVignetteEffect();
		}
	},3.0f,false);

	bAttack=true; // 한번만 작동되도록 제어
}

void AAttackSpiderV2::CheckAttackRange()
{
	toPlayerDistance = FVector::Distance(this->GetActorLocation(), MainCharacter->GetActorLocation());

	if (toPlayerDistance <= AttackRange)
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	
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

void AAttackSpiderV2::SoundControl()
{
	//
	if (!MainCharacter || !SoundControlActor) return;

	FVector SpiderVector=this->GetActorLocation();
	SpiderVector.Z=MainCharacter->GetActorLocation().Z;
	
	float Distance = FVector::Dist(MainCharacter->GetActorLocation(), SpiderVector);
	
	// 거리 비율 계산 (0 ~ 1)
	float DistanceRatio = FMath::Clamp((Distance - SoundControlMinDistance) / (SoundControlMaxDistance - SoundControlMinDistance), 0.0f, 1.0f);
    
	// **비선형 감소 적용 (제곱)** → 가까울수록 더 급격히 볼륨 감소
	float NewVolume = FMath::Clamp(FMath::Pow(DistanceRatio, 2.0f), 0.1f, 1.0f);

	// 배경음 
	SoundControlActor->AudioComp1->SetVolumeMultiplier(NewVolume);
	
	//MainCharacter->AudioComp->SetVolumeMultiplier(1-NewVolume);
	
	if (Distance < 1000.0f) // 플레이어와 가까워지면 볼륨 감소
	{
		MainCharacter->PlayHeartSound();
	}
	else if (Distance > 1000.0f) // 플레이어가 멀어지면 볼륨 증가
	{
		MainCharacter->StopSound();
	}
}

void AAttackSpiderV2::SwitchToMonsterCamera()
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



