// Fill out your copyright notice in the Description page of Project Settings.


#include "middleDoor_Fashion.h"
#include "Components/BoxComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

class ATestCharacter;
// Sets default values
AmiddleDoor_Fashion::AmiddleDoor_Fashion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExitDoor2 = CreateDefaultSubobject<UStaticMeshComponent>("ExitDoor2");
	
	ExitDoor2->SetupAttachment(BoxComp);

	bIsKeyHad = false;

}

// Called when the game starts or when spawned
void AmiddleDoor_Fashion::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AmiddleDoor_Fashion::OnBeginOverlap_);
	
}

// Called every frame
void AmiddleDoor_Fashion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AmiddleDoor_Fashion::RotateDoor()
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

void AmiddleDoor_Fashion::RotateDoor2()
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

void AmiddleDoor_Fashion::StartRotateDoor()
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &AmiddleDoor_Fashion::RotateDoor , 0.01f , true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle2 , this , &AmiddleDoor_Fashion::RotateDoor2 , 0.01f , true);
	
}

void AmiddleDoor_Fashion::DoorOpen()
{
	if (bIsKeyHad)
	{
		StartRotateDoor(); // 열쇠가 있을 때만 문 열림
	}
	else
	{
		//열쇠가 없을 경우 독백 문자 출력
		FString NoEnter =FString(TEXT("<Monologue>문이 닫혀있다... 열쇠 구멍이 있는걸 보니 열쇠가 있으면 열 수 있을 것같다."));
		MainCharacter->PlayerHUD->MonolugueWidgetUI->SetVisibility(ESlateVisibility::Visible);
		MainCharacter->PlayerHUD->MonolugueWidgetUI->SetText_Dialogue(NoEnter);
	}
}

void AmiddleDoor_Fashion::OnBeginOverlap_(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor);

	//문과 부딪히는것이 플레이어일때, 그리고 열쇠를 가지고 있을때 문이 열림
	/*if(TestCharacter && bIsKeyHad)
	{
		StartRotateDoor(); //문 열림
	}*/
	
}

