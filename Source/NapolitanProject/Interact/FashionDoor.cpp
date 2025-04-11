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
	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFashionDoor::OnBeginOverlap);
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




