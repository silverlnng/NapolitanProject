// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoorTrue.h"
#include "GameFramework/Actor.h"
#include "middleDoor_Fashion.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AmiddleDoor_Fashion : public AExitDoorTrue
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AmiddleDoor_Fashion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ExitDoor2; //문이 두개라 두개 필요함
	
	virtual void RotateDoor() override;
	virtual void RotateDoor2();
	virtual void StartRotateDoor();

public:
	FTimerHandle TimerHandle2;

	float TargetYaw2; 
	
	UFUNCTION()
	virtual void OnBeginOverlap_(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool bIsKeyHad;
	
};
