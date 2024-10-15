// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseStatue.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/YJ/TestCharacter.h"


// Sets default values
AChaseStatue::AChaseStatue()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	CSCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CSCol"));
	CSCol->SetCapsuleHalfHeight(90.f);
	CSCol->SetCapsuleRadius(90.f);

}

// Called when the game starts or when spawned
void AChaseStatue::BeginPlay()
{
	Super::BeginPlay();

	Target = CastChecked<ATestCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
}

// Called every frame
void AChaseStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (mState)
	{
	case ChaseStatueState::Idle:
		TickIdle(DeltaTime);
	case ChaseStatueState::Move:
		TickMove(DeltaTime);
	case ChaseStatueState::Attack:
		TickAttack(DeltaTime);
	}
}

// Called to bind functionality to input
void AChaseStatue::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChaseStatue::TickIdle(const float& DeltaTime)
{
	//플레이어가 안에 들어왔고, 그 안에서 자신을 바라보지 않을 때 우선은 대기 상태로 있는다.
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	me-> SetActorRotation(FRotator(0, rot.Yaw, 0));

	//만약 플레이어가 일정 거리 안에 들어왓을 경우 움직이는 모드로 변경
	if (dirR.Size() >= 500.0f)
	{
		mState = ChaseStatueState::Move;
	}
}

void AChaseStatue::TickMove(const float& DeltaTime)
{
	//플레이어가 안에 들어왔고, 그 안에서 자신을 바라보지 않을 때 우선은 대기 상태로 있는다.
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = me->GetActorLocation();
	FVector dirR = targetLoc - myLoc;
	FRotator rot = dirR.Rotation();

	//일정 거리 안에 있을 경우 움직이기
	if (dirR.Size() >= 500.0f)
	{
		me->AddMovementInput(dirR.GetSafeNormal());
	}
	else if (dirR.Size() <= 170.0f)
	{
		//가까울 경우 사망
		mState = ChaseStatueState::Attack;
	}
}

void AChaseStatue::TickAttack(const float& DeltaTime)
{
	//사망 코드 넣기
}



