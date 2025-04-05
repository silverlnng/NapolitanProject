// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFlowerKeyV2.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/Interact/AttachArt.h"

ASunFlowerKeyV2::ASunFlowerKeyV2()
{
	SceneComp2 =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp2"));
	SceneComp2->SetupAttachment(RootComponent);
	
	SkeletalMeshComp1=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp1"));
	SkeletalMeshComp1->SetupAttachment(SceneComp2);
	SkeletalMeshComp2=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp2"));
	SkeletalMeshComp2->SetupAttachment(SceneComp2);
	SkeletalMeshComp3=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp3"));
	SkeletalMeshComp3->SetupAttachment(SceneComp2);
}

void ASunFlowerKeyV2::BeginPlay()
{
	Super::BeginPlay();
	AttachArt = Cast<AAttachArt>(UGameplayStatics::GetActorOfClass(GetWorld(), AAttachArt::StaticClass()));

	//시작할때는 안보이다가 노인 유품을 얻었을 시에만 나오도록
	//SetActorHiddenInGame(true);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("neck_01")),PBO_None);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("thigh_l")),PBO_None);					  SkeletalMeshComp1->HideBoneByName(FName(TEXT("thigh_r")),PBO_None);
	SkeletalMeshComp1->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);

	SkeletalMeshComp2->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);
	SkeletalMeshComp2->HideBoneByName(FName(TEXT("neck_01")),PBO_None);
	SkeletalMeshComp2->HideBoneByName(FName(TEXT("thigh_l")),PBO_None);					  SkeletalMeshComp2->HideBoneByName(FName(TEXT("thigh_r")),PBO_None);
	SkeletalMeshComp2->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);

	SkeletalMeshComp3->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);
	SkeletalMeshComp3->HideBoneByName(FName(TEXT("neck_01")),PBO_None);
	SkeletalMeshComp3->HideBoneByName(FName(TEXT("thigh_l")),PBO_None);					  SkeletalMeshComp3->HideBoneByName(FName(TEXT("thigh_r")),PBO_None);
	SkeletalMeshComp3->HideBoneByName(FName(TEXT("clavicle_r")),PBO_None);

	//SkeletalMeshComp3->UnHideBoneByName()
}

void ASunFlowerKeyV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASunFlowerKeyV2::OnPickup()
{

	/*FVector OriginLoc = GetActorLocation();
	FRotator OriginRot=GetActorRotation();
	
	GetWorldTimerManager().SetTimer(RepeatTimerHandle,[this]()
	{
		FVector CameraLoc =MainCharacter->CenterArrowComp->GetComponentLocation();
		FRotator CameraRot =MainCharacter->CenterArrowComp->GetComponentRotation();
		SetActorLocationAndRotation(CameraLoc,CameraRot);
	},0.1f,true);
	
	GetWorldTimerManager().SetTimer(StopTimerHandle,[this]()
	{
		GetWorldTimerManager().ClearTimer(RepeatTimerHandle);
	},5.0f,false);*/

	// 카메라에 가까이 , 특정각도로, 그리고
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	AttachToComponent(MainCharacter->CenterArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	

	// 몇초뒤 흔들림 효과+ 팔 뻗어나오기
	
	
	// 아님점프스케어 연출뒤에 Super::OnPickup 작동되도록 만들기 
	// Super::OnPickup(); 
	
	
}

void ASunFlowerKeyV2::PutDown()
{
	Super::PutDown();
}
