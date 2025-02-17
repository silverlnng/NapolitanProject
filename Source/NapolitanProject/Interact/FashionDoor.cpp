// Fill out your copyright notice in the Description page of Project Settings.


#include "FashionDoor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
AFashionDoor::AFashionDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFashionDoor::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AFashionDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFashionDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFashionDoor::RotateDoor()
{
	Super::RotateDoor();
}

void AFashionDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//만약 부딪힌게 캐릭터일 경우 Frame은 아래로 내려감
	if(OtherActor)
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
	}

}



