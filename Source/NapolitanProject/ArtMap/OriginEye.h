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
	class APlayerHUD* PlayerHUD;

	void SetEyeVisible(bool bVisible); //눈을 보이게 하는 함수

private:
	bool bShouldLookAtPlayer;
	FRotator RandomLookDirection;
	float TimeSinceLastRandomLook;
	float RandomLookInterval;
	float RandomSpeed;

public:
	// 추격 상태
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chase")
	bool bIsChasing;
    
	// 추격 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chase")
	float ChaseSpeed = 800.0f;
    
	// 가속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chase")
	float ChaseAcceleration = 400.0f;
    
	// 현재 속도
	FVector CurrentVelocity;

public:
	// 추격 로직
	void UpdateChasing(float DeltaTime);

	UPROPERTY(EditAnywhere)
	FString description=FString(TEXT("수많은 눈동자들에 의해 사망했다"));

	//각 눈알의 고유한 목표 오프셋
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chase")
	FVector UniqueTargetOffset;
	
	static float StopChaseDelay;
	static bool bIsDeadEnding;
	static bool bStopChase;
};
