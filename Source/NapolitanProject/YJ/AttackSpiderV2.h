// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpiderV2.generated.h"

UENUM(BlueprintType)
enum class EAttackSpiderV2State:uint8
{
	Move,
	Drop,
	Chase,
	Attack
};

UCLASS()
class NAPOLITANPROJECT_API AAttackSpiderV2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAttackSpiderV2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
public:
	void MoveAlongSpline(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* AIPerception;
	
	void AttackPlayer();

	void DetectAndDrop();

	void StartChasing();

	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void CheckAttackRange();

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	class AActor* SplineActor;
	UPROPERTY(EditAnywhere, Category = "Movement")
 	class USplineComponent* CurrentSpline; // 따라갈 Spline

public:
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 200.0f; // 이동 속도

	float DistanceAlongSpline = 0.0f; // 현재 이동 거리
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadWrite)
	float Distance;
	
	class AAIController* AIController;

	UPROPERTY()
	class UAttackSpider_AnimInstance* Anim;
	
	UPROPERTY(VisibleAnywhere)
	EAttackSpiderV2State CurrentState= EAttackSpiderV2State::Move;
	
	UFUNCTION()
	void SetAIState(EAttackSpiderV2State NewState);

	UPROPERTY()
	FTimerHandle ChaseCheckTimer;
};

