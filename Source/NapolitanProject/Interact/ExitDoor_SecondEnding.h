// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoorTrue.h"
#include "ExitDoor_SecondEnding.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AExitDoor_SecondEnding : public AExitDoorTrue
{
	GENERATED_BODY()

	AExitDoor_SecondEnding();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void RotateDoor() override;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> DetectiveLevel;
	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp2; // 문 안쪽에 설치할 트리거 
};
