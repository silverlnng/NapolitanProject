// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "PieceActor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APieceActor : public AItemActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 PieceNum=0;

	virtual void OnPickup() override; 
};
