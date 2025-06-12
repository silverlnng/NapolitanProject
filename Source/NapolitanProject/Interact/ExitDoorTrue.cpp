// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoorTrue.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "../GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/SaveGISubsystem.h"

// Sets default values
AExitDoorTrue::AExitDoorTrue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(RootComponent);

	ExitDoor = CreateDefaultSubobject<UStaticMeshComponent>("ExitDoor");
	ExitDoor->SetupAttachment(BoxComp);

	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Block);
	

}

// Called when the game starts or when spawned
void AExitDoorTrue::BeginPlay()
{
	Super::BeginPlay();

	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}
	GI =GetGameInstance<UMyTestGameInstance>();
	
	SaveGI=GI->GetSubsystem<USaveGISubsystem>();
}

// Called every frame
void AExitDoorTrue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitDoorTrue::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	/*if(OtherActor)
	{
		if(bIsOpenDoor) // 한번만 열리도록 만든 것
			return;
		
		auto* Target = Cast<ATestCharacter>(OtherActor);
		if(Target)
		{
			bIsOpenDoor = true;
			if (OpenDoorSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), OpenDoorSound); // 소리 재생
			}
			
			// 목표 Yaw 계산 (현재 Yaw에서 90도 추가)
			FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
			TargetYaw = CurrentRotation.Yaw + 90.0f;

			// 타이머 설정: 문을 부드럽게 회전
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExitDoorTrue::RotateDoor, 0.01f, true);
		}
	}*/
}


void AExitDoorTrue::RotateDoor()
{
	
	FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
	float InterpSpeed = 2.0f; // 회전 속도 조절

	// FMath::FInterpTo로 현재 Yaw에서 목표 Yaw까지 이동
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	// 새로운 회전 값 설정
	ExitDoor->SetRelativeRotation(CurrentRotation);

	// 목표 각도 도달 여부 확인
	if (FMath::Abs(CurrentRotation.Yaw - TargetYaw) < 0.5f)
	{
		// 목표 각도에 도달하면 회전 종료
		ExitDoor->SetRelativeRotation(FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll)); // 정확히 목표 각도로 설정
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
void AExitDoorTrue::IInteract()
{
	DoorOpen();
}

void AExitDoorTrue::DoorOpen()
{
	//상호작용 키(E키)를 눌렀을 경우 호출
	
	if(bIsOpenDoor) // 한번만 열리도록 만든 것
		return;
		
	bIsOpenDoor = true;
	if (OpenDoorSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OpenDoorSound); // 소리 재생
	}
			
	// 목표 Yaw 계산 (현재 Yaw에서 90도 추가)
	FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
	TargetYaw = CurrentRotation.Yaw + 90.0f;

	// 타이머 설정: 문을 부드럽게 회전
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AExitDoorTrue::RotateDoor, 0.01f, true);

}

