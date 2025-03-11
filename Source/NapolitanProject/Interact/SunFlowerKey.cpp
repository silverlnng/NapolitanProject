// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFlowerKey.h"

#include "AttachArt.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
ASunFlowerKey::ASunFlowerKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASunFlowerKey::BeginPlay()
{
	Super::BeginPlay();
	
	AttachArt = Cast<AAttachArt>(UGameplayStatics::GetActorOfClass(GetWorld(), AAttachArt::StaticClass()));

	//시작할때는 안보이다가 노인 유품을 얻었을 시에만 나오도록
	SetActorHiddenInGame(true);

}

// Called every frame
void ASunFlowerKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASunFlowerKey::OnPickup()
{
	Super::OnPickup();

	StaticMeshComp->SetRelativeRotation(FRotator(90,-90,0));
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3,ECR_Ignore);
	OnInventorySlot();
	//들고 다니기, 아이템에서 기본 상속
	AttachToComponent(MainCharacter->ItemArrowComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bIsPickUp = true; //픽업했을 시에 true
}

void ASunFlowerKey::PutDown()
{
	Super::PutDown();
	
}

