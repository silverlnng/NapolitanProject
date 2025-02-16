// Fill out your copyright notice in the Description page of Project Settings.


#include "FashionDoor.h"

// Sets default values
AFashionDoor::AFashionDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

