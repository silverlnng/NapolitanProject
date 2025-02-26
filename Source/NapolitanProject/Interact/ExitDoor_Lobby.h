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

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ExitDoor2;
	
	virtual void RotateDoor() override;
	virtual void RotateDoor2();
	virtual void StartRotateDoor();

	FTimerHandle TimerHandle2;

	float TargetYaw2;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> StartLevel;
	UPROPERTY()
	class UMyTestGameInstance* GI;
	
	UFUNCTION()
	virtual void OnBeginOverlap_(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
