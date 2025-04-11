// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interact/ExitDoorTrue.h"
#include "EyesDoor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AEyesDoor : public AExitDoorTrue
{
	GENERATED_BODY()
public:
	AEyesDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void RotateDoor() override;

	virtual void DoorOpen() override;

public:
	
	bool GetDoorOpen; //문을 여는 변수, true일때 열린다.


public:

	
	
};
