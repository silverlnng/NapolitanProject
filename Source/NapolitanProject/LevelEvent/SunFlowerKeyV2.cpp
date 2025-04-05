// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFlowerKeyV2.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/AttachArt.h"

void ASunFlowerKeyV2::BeginPlay()
{
	Super::BeginPlay();
	AttachArt = Cast<AAttachArt>(UGameplayStatics::GetActorOfClass(GetWorld(), AAttachArt::StaticClass()));

	//시작할때는 안보이다가 노인 유품을 얻었을 시에만 나오도록
	//SetActorHiddenInGame(true);
}

void ASunFlowerKeyV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASunFlowerKeyV2::OnPickup()
{

	FVector OriginLoc = GetActorLocation();
	FRotator OriginRot=GetActorRotation();
	FVector CameraLoc =MainCharacter->CenterArrowComp->GetComponentLocation();
	FRotator CameraRot =MainCharacter->CenterArrowComp->GetComponentRotation();
	FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer,[this, CameraLoc, CameraRot]()
	{
		SetActorLocationAndRotation(CameraLoc,CameraRot);
	},0.5f,false);
	

	// 카메라에 가까이 , 특정각도로, 그리고

	// 몇초뒤 흔들림 효과+ 팔 뻗어나오기
	
	
	// 아님점프스케어 연출뒤에 Super::OnPickup 작동되도록 만들기 
	// Super::OnPickup(); 
	
	
}

void ASunFlowerKeyV2::PutDown()
{
	Super::PutDown();
}
