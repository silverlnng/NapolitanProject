// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoor_Lobby.h"
#include "Door_2Floor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ADoor_2Floor : public AExitDoor_Lobby
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	void BindBeginOverlap();

	void UnBindBeginOverlap();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void RotateDoor() override;
	virtual void RotateDoor2()override;
	virtual void StartRotateDoor()override;
	
	void JustRotateDoor();
	
};
