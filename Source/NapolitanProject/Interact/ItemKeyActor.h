// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ItemKeyActor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AItemKeyActor : public AItemActor
{
	GENERATED_BODY()
public:	
	virtual void BeginPlay() override;
	
	virtual void OnPickup() override;
	
};
