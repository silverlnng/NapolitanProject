// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OriginEye.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AOriginEye : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOriginEye();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* EyeMesh;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* EyeSphere;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* Arrow; //플레이어 방향으로 바라보게 하는 용도

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	void SetEyeVisible(bool bVisible); //눈을 보이게 하는 함수

private:
	bool bShouldLookAtPlayer;
	FRotator RandomLookDirection;
	float TimeSinceLastRandomLook;
	float RandomLookInterval;
	float RandomSpeed;

};
