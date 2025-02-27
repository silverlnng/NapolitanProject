// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "GameFramework/Actor.h"
#include "Item_MiddleDoorKey.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AItem_MiddleDoorKey : public AItemActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_MiddleDoorKey();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

public:	
	virtual void OnPickup() override;
	

};
