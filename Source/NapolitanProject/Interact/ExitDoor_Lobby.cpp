// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_Lobby.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AExitDoor_Lobby::AExitDoor_Lobby()
{
	ExitDoor2 = CreateDefaultSubobject<UStaticMeshComponent>("ExitDoor2");
	
	ExitDoor2->SetupAttachment(BoxComp);
}


void AExitDoor_Lobby::RotateDoor()
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

void AExitDoor_Lobby::RotateDoor2()
{
		
	FRotator CurrentRotation = ExitDoor2->GetRelativeRotation();
	float InterpSpeed = 2.0f; // 회전 속도 조절

	// FMath::FInterpTo로 현재 Yaw에서 목표 Yaw까지 이동
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw2, GetWorld()->GetDeltaSeconds(), InterpSpeed);

	// 새로운 회전 값 설정
	ExitDoor2->SetRelativeRotation(CurrentRotation);

	// 목표 각도 도달 여부 확인
	if (FMath::Abs(CurrentRotation.Yaw - TargetYaw2) < 0.5f)
	{
		// 목표 각도에 도달하면 회전 종료
		ExitDoor2->SetRelativeRotation(FRotator(CurrentRotation.Pitch, TargetYaw2, CurrentRotation.Roll)); // 정확히 목표 각도로 설정
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle2);
	}
}

void AExitDoor_Lobby::StartRotateDoor()
{
	if(bIsOpenDoor) // 한번만 열리도록 만든 것
		return;

	bIsOpenDoor = true;
	if (OpenDoorSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld() , OpenDoorSound); // 소리 재생
	}

	// 목표 Yaw 계산 (현재 Yaw에서 90도 추가)
	FRotator CurrentRotation = ExitDoor->GetRelativeRotation();
	TargetYaw = CurrentRotation.Yaw + 90.0f;
	
	TargetYaw2 = CurrentRotation.Yaw - 90.0f;

	// 타이머 설정: 문을 부드럽게 회전
	GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &AExitDoor_Lobby::RotateDoor , 0.01f , true);
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandle2 , this , &AExitDoor_Lobby::RotateDoor2 , 0.01f , true);
	
}
