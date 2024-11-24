// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_First.h"

#include "Components/BoxComponent.h"

void AExitDoor_First::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitDoorTrue::OnBeginOverlap);
}

void AExitDoor_First::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
}

void AExitDoor_First::RotateDoor()
{
	Super::RotateDoor();
}
