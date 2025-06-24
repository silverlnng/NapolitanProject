// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Spector.generated.h"

UENUM(BlueprintType)
enum class ESpectorState : uint8
{
	Watch,
	Move
};

UCLASS()
class NAPOLITANPROJECT_API ASpector : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = State)
	ESpectorState mState = ESpectorState::Watch; //기본은 관람 상태

	//상태 변경 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void SetState(ESpectorState newstate);

	//상태 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void TickWatch(const float& DeltaTime); //관람 상태

	UFUNCTION(BlueprintCallable, Category = State)
	void TickMove(const float& DeltaTime); //움직이는 상태

public:
	UPROPERTY()
	class AAIController* AI;

	float CurrentTime = 0; //현재 시간
	float IdleDelayTime = 5.0f; //대기 시간

	bool bIsMoving = false;

	FVector TargetPoint; //타겟 포인트 지점
	
	// 기존 TargetPoint 아래에 추가
	FVector LastVisitedPoint = FVector::ZeroVector; // 마지막 방문한 위치

public:
	UPROPERTY()
	bool bIsRotating = false;
    
	UPROPERTY()
	FRotator TargetRotation;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spector Settings")
	float RotationSpeed = 2.0f; // 회전 속도

public:
	UPROPERTY()
	class USpectatorAnim* Anim;
	
};
