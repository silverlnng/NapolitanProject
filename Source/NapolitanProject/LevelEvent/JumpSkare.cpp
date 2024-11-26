// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpSkare.h"

#include "BDTrigger.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AJumpSkare::AJumpSkare()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AJumpSkare::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AJumpSkare::AJumpSkare::BeginOverlap);
	
	//GetMesh()->SetVisibility(false);

	// 이동 속도 조정
	GetCharacterMovement()->MaxWalkSpeed = JumpSkareMaxSpeed; // 기본 속도보다 빠르게 설정
}

// Called every frame
void AJumpSkare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsJumpSkare)
	{
		GetMesh()->SetVisibility(true);

		//앞으로 빠르게 달려오기
		FVector ForwardDirection = GetActorForwardVector(); // 캐릭터의 앞 방향 벡터
		//AddMovementInput(ForwardDirection, 1.0f); // 앞으로 이동
	}

}

// Called to bind functionality to input
void AJumpSkare::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AJumpSkare::JumpSkareStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("JumpSkare Start"));


	//불 깜빡깜빡하는 효과 넣기

	//그다음 달려오기
	bIsJumpSkare = true;
	
}

void AJumpSkare::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("JumpSkare Overlap"));
	if(OtherActor)
	{
		AActor* TriggerActor = Cast<ABDTrigger>(OtherActor);
		if(TriggerActor)
		{
			//캐릭터가 트리거 박스 위치로 왓을 경우 사라지기
			GetMesh()->SetVisibility(false);
		}
	}
}

