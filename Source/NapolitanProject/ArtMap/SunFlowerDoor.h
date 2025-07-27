// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interact/LevelMoveDoor/LevelMoveDoor.h"
#include "SunFlowerDoor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASunFlowerDoor : public ALevelMoveDoor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASunFlowerDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpenKey;
	
public:
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
