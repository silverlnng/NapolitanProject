// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueActor.h"

#include "../GameFrameWork/PlayerHUD.h"
#include "../GameFrameWork/TestCharacter.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AClueActor::AClueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(FName("Clue"));

	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	TextRenderComp=CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComp"));
	TextRenderComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void AClueActor::BeginPlay()
{
	Super::BeginPlay();
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	MainCharacter =TestPC->GetPawn<ATestCharacter>();
	PlayerHUD =TestPC->GetHUD<APlayerHUD>();
}

// Called every frame
void AClueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClueActor::LookAt()
{
	// 캐릭터의 카메라 시점으로 이동 시켰다가 제자리로
	FVector OriginLoc = GetActorLocation();
	FRotator OriginRot=GetActorRotation();
	FVector CameraLoc =MainCharacter->myArrowComp->GetComponentLocation();
	FRotator CameraRot =MainCharacter->myArrowComp->GetComponentRotation();
	FTimerHandle UITimer;
	GetWorldTimerManager().SetTimer(UITimer,[this, CameraLoc, CameraRot]()
	{
		SetActorLocationAndRotation(CameraLoc,CameraRot);
	},0.5f,false);

	/*FTimerHandle OriginPoseTimer;
	GetWorldTimerManager().SetTimer(OriginPoseTimer,[this,OriginLoc, OriginRot]()
	{
		SetActorLocationAndRotation(OriginLoc,OriginRot);
	},5.f,false);*/

	// 쪽지 ui 가 나오도록 하기 
	
}