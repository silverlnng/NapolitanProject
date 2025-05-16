// Fill out your copyright notice in the Description page of Project Settings.


#include "Picture_Zombie.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
APicture_Zombie::APicture_Zombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	SetRootComponent(BoxComp);
	//BoxComp->SetupAttachment(RootComponent);

	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);
	
	CapsuleComp=CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(StaticMeshComp);
	
	SkeletalMeshComp1=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp1"));
	SkeletalMeshComp1->SetupAttachment(BoxComp);
	
	HeadSocketCapsuleComp=CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsuleComp"));
	HeadSocketCapsuleComp->SetupAttachment(SkeletalMeshComp1,FName(TEXT("HeadSocket")));

	SpineSocketCapsuleComp=CreateDefaultSubobject<UCapsuleComponent>(TEXT("SpineCapsuleComp"));
	SpineSocketCapsuleComp->SetupAttachment(SkeletalMeshComp1,FName(TEXT("Spine2Socket")));

	MonsterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MonsterCamera"));
	MonsterCamera->SetupAttachment(SkeletalMeshComp1,FName(TEXT("HeadSocket"))); // 루트에 부착
	MonsterCamera->bUsePawnControlRotation = false; // 플레이어 조작 방지
}

// Called when the game starts or when spawned
void APicture_Zombie::BeginPlay()
{
	Super::BeginPlay();

	// 스켈레탈 메쉬는 아예 안보이도록

	SkeletalMeshComp1->SetHiddenInGame(true);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("RightUpLeg")),PBO_None);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("LeftUpLeg")),PBO_None);


	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();
}

// Called every frame
void APicture_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Location = CapsuleComp->GetComponentLocation();
	float HalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
	float Radius = CapsuleComp->GetUnscaledCapsuleRadius();
	FRotator Rotation = CapsuleComp->GetComponentRotation();

	DrawDebugCapsule(
		GetWorld(),
		Location,
		HalfHeight,
		Radius,
		Rotation.Quaternion(),
		FColor::Green,
		false,       // 지속 여부 (true면 무한)
		-1.f,        // 지속 시간 (-1이면 한 프레임)
		0,           // Depth Priority
		1.f          // 두께
	);
}

 void APicture_Zombie::DropEffect()
{
	BoxComp->SetSimulatePhysics(true);
	
	// 몇초 후 좀비의 상반신 만 보이도록 수정
	
	
	FTimerHandle nextTimer;
	GetWorld()->GetTimerManager().SetTimer(nextTimer,[this]()
	{
		//BoxComp->SetSimulatePhysics(false);
		ZombieStart();
	},4.0f,false);

	
}

void APicture_Zombie::ZombieStart()
{
	// 콜리전이 설정되도록
	//BoxComp->SetSimulatePhysics(false);
	//BoxComp->SetCenterOfMass(FVector3d(0,0,-10));

	//BoxComp->SetConstraintMode( )
	
	SkeletalMeshComp1->SetHiddenInGame(false);
	
	CapsuleComp->SetSimulatePhysics(true);
	// 애니메이션 실행
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	
	GetWorldTimerManager().SetTimer(
	  CapsuleGrowTimer,
	  this,
	  &APicture_Zombie::UpdateCapsuleGrowth,
	  TimerInterval,
	  true // 반복
  );
	
}

void APicture_Zombie::UpdateCapsuleGrowth()
{
	ElapsedTime += TimerInterval;

	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.f);
	float NewHeight = FMath::Lerp(OriginYRot, TargetHeight, Alpha);

	CapsuleComp->SetCapsuleHalfHeight(NewHeight);

	
	if (Alpha >= 1.0f)
	{
		GetWorldTimerManager().ClearTimer(CapsuleGrowTimer);
		CapsuleComp->SetCapsuleHalfHeight(TargetHeight); // 보정
		
		
		//SetActorRotation(FRotator(TargetYrot,TargetZrot,TargetXrot));
		
		BoxComp->SetSimulatePhysics(false);
	
		CapsuleComp->SetSimulatePhysics(false);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		InitialRotation = GetActorRotation();
		StartPitch=InitialRotation.Pitch;
		SetActorRotation(FRotator(TargetPitch,TargetYaw,InitialRotation.Roll));
		// 다음함수 진행
		//UpdateRotationStart();

		if (!bHasStartedMoving)
		{
			bHasStartedMoving = true;
			StartMovingForward();
		}
	}
}

void APicture_Zombie::ChasePlayer()
{
	
}

void APicture_Zombie::UpdateRotationStart()
{
	InitialRotation = GetActorRotation();
	StartPitch=InitialRotation.Pitch;
	FTimerHandle LookAtTimer;
	GetWorldTimerManager().SetTimer(LookAtTimer, this, &APicture_Zombie::UpdateRotation, 0.02f, true);
}

void APicture_Zombie::UpdateRotation()
{
	ElapsedTime += TimerInterval;

	float Alpha = FMath::Clamp(ElapsedTime / TotalTime, 0.f, 1.f);
	float NewYaw = FMath::Lerp(StartYaw, TargetYaw, Alpha);
	float NewPitch=FMath::Lerp(StartPitch, TargetPitch, Alpha);
	
	FRotator NewRotation = InitialRotation;
	NewRotation.Yaw = NewYaw;
	NewRotation.Pitch=NewPitch;
	
	SetActorRotation(NewRotation);

	if (Alpha >= 1.0f)
	{
		GetWorldTimerManager().ClearTimer(RotationTimer);
		SetActorRotation(FRotator(TargetPitch,TargetYaw,InitialRotation.Roll)); // 최종 고정
		
		// 앞으로 움직임 시작
		if (!bHasStartedMoving)
		{
			bHasStartedMoving = true;
			StartMovingForward();
		}
	}
}

void APicture_Zombie::StartMovingForward()
{

	UE_LOG(LogTemp, Warning, TEXT("StartMovingForward called!"));
	
	MoveDirection = GetActorForwardVector().GetSafeNormal();
	
	// 타이머 시작
	GetWorldTimerManager().SetTimer(
		MoveForwardTimer,
		this,
		&APicture_Zombie::MoveForwardStep,
		MoveInterval,
		true
	);
}

void APicture_Zombie::MoveForwardStep()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveForwardStep called!"));
	
	// === 거리 체크 먼저 ===
	float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());
	
	if (!bHasTriggered && Distance <= TriggerDistance)
	{
		bHasTriggered = true;
		// ✅ 타이머 멈춤 → 이동 정지
		GetWorldTimerManager().ClearTimer(MoveForwardTimer);
		MainCharacter->SetActorHiddenInGame(true);
	
		// 사망이벤트 만 발생시킴
		MainCharacter->bIsBeingAttacked=true;
		// ✅ 이벤트 발생
		TriggerScareEvent();
		return; // 이동도 하지 않음!
	}

	// === 이동 ===
	FVector CurrentLocation = GetActorLocation();
	FVector DeltaMove = MoveDirection * MoveSpeed * MoveInterval;
	
	SetActorLocation(GetActorLocation() + DeltaMove);
	
}

void APicture_Zombie::TriggerScareEvent()
{
	FTimerHandle SwitchCameraTimer;

	GetWorld()->GetTimerManager().SetTimer(SwitchCameraTimer,[this]()
	{
	},0.75f,false);
	SwitchToMonsterCamera();
	FTimerHandle UITimer2;
	GetWorld()->GetTimerManager().SetTimer(UITimer2,[this]()
	{
		if (PlayerHUD )
		{
		
			PlayerHUD->PlayDeadVignetteEffect();
		}
	},2.5f,false);
}

void APicture_Zombie::SwitchToMonsterCamera()
{
	if (TestPC && MonsterCamera)
	{
		// 카메라 전환
		TestPC->SetViewTargetWithBlend(this, 0.01f); // 0.5초 동안 부드럽게 전환
		
	}
}





