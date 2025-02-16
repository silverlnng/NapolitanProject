// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoorTrue.h"
#include "GameFramework/Actor.h"
#include "FashionDoor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AFashionDoor : public AExitDoorTrue
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFashionDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
