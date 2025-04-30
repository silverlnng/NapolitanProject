// Fill out your copyright notice in the Description page of Project Settings.


#include "Docent_JumpScare.h"

#include "DeadEndingWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/NPC_Docent_AnimInstance.h"

// Sets default values
ADocent_JumpScare::ADocent_JumpScare()
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
void ADocent_JumpScare::BeginPlay()
{
	Super::BeginPlay();
	
	// 처음에는 안보이도록
	SetActorHiddenInGame(true);
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();

	Anim=Cast<UNPC_Docent_AnimInstance>(GetMesh()->GetAnimInstance());
	
	if (SplineActor)
	{
		CurrentSpline=Cast<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
	}
	if (CurrentSpline==nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentSpline Null")));
	}

	
}

// Called every frame
void ADocent_JumpScare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving && CurrentSpline)
	{
		MoveAlongSpline(DeltaTime);
	}
}

void ADocent_JumpScare::MoveAlongSpline(float DeltaTime)
{
	if (!CurrentSpline) return;

	// 이동 거리 증가
	DistanceAlongSpline += SplineMoveSpeed * DeltaTime;
	
	float SplineLength = CurrentSpline->GetSplineLength();

	// 🎯 Spline 끝에 도달하면  => 움직임 멈추고 점프스케어 시작하기 
	if (DistanceAlongSpline >= SplineLength)
	{
		bIsMoving=false;
		// 캐릭터 움직이지 못하게 하고 점프스케어 시작하기
		StartAttack();
	}

	// 새로운 위치 및 회전 계산
	FVector NewLocation = CurrentSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator NewRotation = CurrentSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void ADocent_JumpScare::StartMoving()
{
	if (CurrentSpline)
	{
		DistanceAlongSpline = 0.0f;
		bIsMoving = true;
		Anim->bIsMoving=true;
	}
}

void ADocent_JumpScare::SwitchToMonsterCamera()
{
	if (TestPC && MonsterCamera)
	{
		// 카메라 전환
		TestPC->SetViewTargetWithBlend(this, 0.01f); // 0.5초 동안 부드럽게 전환

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

void ADocent_JumpScare::StartAttack()
{
	// 한번만 작동되도록 
	if (bAttack){return;}

	//공격소리로 바꾸기
	Anim->bIsMoving=false;
	Anim->bAttackStart=true;

	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
	
	MainCharacter->SetActorHiddenInGame(true);
	
	// 사망이벤트 만 발생시킴
	MainCharacter->bIsBeingAttacked=true;
	//카메라 쉐이크 .

	FTimerHandle SwitchCameraTimer;

	GetWorld()->GetTimerManager().SetTimer(SwitchCameraTimer,[this]()
	{
		SwitchToMonsterCamera();
	},0.75f,false);

	
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
			/*FString name= FString(TEXT("<Red_Big>도슨트 에게</>"));
			PlayerHUD->DeadEndingWidgetUI->SetRichText_Name(name);
			PlayerHUD->DeadEndingWidgetUI->StartLerpTimer();*/
		}
	},3.5f,false);

	bAttack=true; // 한번만 작동되도록 제어
}


