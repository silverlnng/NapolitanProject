// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interact/LevelMoveDoor.h"
#include "SunFloorDoorToLobby.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ASunFloorDoorToLobby : public ALevelMoveDoor
{
	GENERATED_BODY()

public:
	ASunFloorDoorToLobby();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bOneMove;
	
	
};
