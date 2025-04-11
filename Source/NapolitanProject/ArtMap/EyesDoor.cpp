// Fill out your copyright notice in the Description page of Project Settings.


#include "EyesDoor.h"

#include "Kismet/GameplayStatics.h"

AEyesDoor::AEyesDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEyesDoor::BeginPlay()
{
	Super::BeginPlay();

	GetDoorOpen = false;
}

void AEyesDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEyesDoor::RotateDoor()
{
	Super::RotateDoor();
}

void AEyesDoor::DoorOpen()
{
	//UI에서 비번을 맞췄을때 열리도록
	if(GetDoorOpen)
	{
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

		//문이 열리면 제한시간 타이머 초기화하고 사망 이벤트 없어짐
	}
}
