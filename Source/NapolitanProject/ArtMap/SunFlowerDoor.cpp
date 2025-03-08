// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFlowerDoor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/YJ/Monologue/MonolugueWidget.h"

// Sets default values
ASunFlowerDoor::ASunFlowerDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ASunFlowerDoor::BeginPlay()
{
	Super::BeginPlay();
	
	bIsOpenKey = false;

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASunFlowerDoor::BeginOverlap);
}

// Called every frame
void ASunFlowerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASunFlowerDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsOpenKey) return; // 키가 없으면 아무 동작도 하지 않음

	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}





