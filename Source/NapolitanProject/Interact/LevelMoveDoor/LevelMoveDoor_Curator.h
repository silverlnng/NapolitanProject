// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelMoveDoor.h"
#include "LevelMoveDoor_Curator.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ALevelMoveDoor_Curator : public ALevelMoveDoor
{
	GENERATED_BODY()
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void LevelMove() override;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> FinalLevel;
};
