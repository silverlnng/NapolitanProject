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
	BaseYaw = GetActorRotation().Yaw;

	SkeletalMeshComp1->SetHiddenInGame(true);
	SkeletalMeshComp2->SetHiddenInGame(true);
	SkeletalMeshComp3->SetHiddenInGame(true);
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
	
	// 2초뒤에 독백대사 나오도록 하기
	FTimerHandle MonologueTimer;

	GetWorld()->GetTimerManager().SetTimer(MonologueTimer , [this]()
	{
		
	} , 1.0f , false);

	// 2.5초뒤 흔들림 효과+ 팔 뻗어나오기 효과 연출
	FTimerHandle effectTimer;

	GetWorld()->GetTimerManager().SetTimer(effectTimer , [this]()
	{
		StartShake();

		// 스켈레탈메쉬 보이도록 하고
		SkeletalMeshComp1->SetHiddenInGame(false);
		SkeletalMeshComp2->SetHiddenInGame(false);
		SkeletalMeshComp3->SetHiddenInGame(false);
		// 애니메이션 작동
		
		
	} , 2.5f , false);

	//3.5초 뒤 효과 중지하도록 하기
	FTimerHandle effectEndTimer;
	GetWorld()->GetTimerManager().SetTimer(effectEndTimer , [this]()
	{
		SkeletalMeshComp1->SetHiddenInGame(true);
		SkeletalMeshComp2->SetHiddenInGame(true);
		SkeletalMeshComp3->SetHiddenInGame(true);
		
	} , 4.0f , false);
	
	// 아님점프스케어 연출뒤에 Super::OnPickup 작동되도록 만들기 
	// Super::OnPickup(); 
	
	
}

void ASunFlowerKeyV2::PutDown()
{
	Super::PutDown();
}

void ASunFlowerKeyV2::StartShake()
{
	// 타이머시작 => 흔들림효과 시작
	GetWorldTimerManager().SetTimer(StartTimerHandle, this, &ASunFlowerKeyV2::UpdateShake, 0.1f, true);

	// 타이머 중지
	GetWorldTimerManager().SetTimer(StopTimerHandle, this, &ASunFlowerKeyV2::StopShake, 2.0f, false);
}

void ASunFlowerKeyV2::UpdateShake()
{
	TimeAccumulator += GetWorld()->GetTimeSeconds();
	float AngleOffset = FMath::Sin(TimeAccumulator * SwingSpeed) * SwingAmplitude;

	FRotator NewRot = FRotator(0, BaseYaw + AngleOffset, 0);
	StaticMeshComp->SetRelativeRotation(NewRot);
}

void ASunFlowerKeyV2::StopShake()
{
	GetWorldTimerManager().ClearTimer(StartTimerHandle);
}
