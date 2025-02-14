// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoorTrue.h"
#include "ExitDoor_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AExitDoor_Lobby : public AExitDoorTrue
{
	GENERATED_BODY()
	
public:
	AExitDoor_Lobby();
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ExitDoor2;
	
	
	void BindBeginOverlap();

	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void RotateDoor() override;
	void RotateDoor2();
	void StartRotateDoor();
};
