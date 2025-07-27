// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemActor.h"
#include "PieceActor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APieceActor : public AItemActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 PieceNum=0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Picked =false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* PickUpSoundWave;
	
	virtual void OnPickup() override;
	
};
