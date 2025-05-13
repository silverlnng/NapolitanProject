// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Picture_Zombie.generated.h"

UCLASS()
class NAPOLITANPROJECT_API APicture_Zombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APicture_Zombie();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp1;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* SkCapsuleComp;
	
	void DropEffect();

	void ZombieStart();

	void UpdateCapsuleGrowth();
	
	FTimerHandle CapsuleGrowTimer;

	float StartHeight;
	UPROPERTY(EditAnywhere,Category="CapsuleGrowTimer")
	float TargetHeight = 40.f;
	UPROPERTY(EditAnywhere,Category="CapsuleGrowTimer")
	float Duration = 2.0f;
	UPROPERTY(EditAnywhere,Category="CapsuleGrowTimer")
	float SkCapsuleTargetLoc = 175.f;
	
	float ElapsedTime = 0.0f;
	float TimerInterval = 0.02f; // 50fps 정도

	void ChasePlayer();

	void UpdateRotationStart();
	void UpdateRotation();
	
	FTimerHandle RotationTimer;

	float StartYaw = 0.f;
	UPROPERTY(EditAnywhere,Category="RotationTimer")
	float TargetYaw = 90.f;
	float StartPitch = 0.f;
	float TargetPitch = 0.f;
	float RotElapsedTime = 0.f;
	UPROPERTY(EditAnywhere,Category="RotationTimer")
	float TotalTime = 2.0f; // 2초 동안 회전
	float RotTimerInterval = 0.02f;
	FRotator InitialRotation;

	void StartMovingForward();

	bool bHasStartedMoving = false;
	
	FTimerHandle MoveForwardTimer;

	FVector MoveDirection; // 이동 방향
	UPROPERTY(EditAnywhere,Category="MoveForwardTimer")
	float MoveSpeed = 300.f; // 1초당 300cm (3m)
	float MoveInterval = 0.02f; // 50 FPS 정도

	void MoveForwardStep();
};


