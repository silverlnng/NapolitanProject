// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor_LeeSeo.h"
#include "Components/BoxComponent.h"

void AExitDoor_LeeSeo::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExitDoorTrue::OnBeginOverlap);
}

void AExitDoor_LeeSeo::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep , SweepResult);
}

void AExitDoor_LeeSeo::RotateDoor()
{
	Super::RotateDoor();
}
