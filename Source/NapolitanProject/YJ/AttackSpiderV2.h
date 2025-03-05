// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpiderV2.generated.h"

struct FAIStimulus;
class UAIPerceptionComponent;

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
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere, Category = "AI")
	float HearingRange = 1000.0f; // 몬스터의 청각 감지 범위

	UPROPERTY(EditAnywhere, Category = "AI")
	float HearingZRange = 500.0f;  // 🎯 Z축 감지 범위 확장
	
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bUseLoSHearing = true; // 선형 시야(Line of Sight) 사용 여부

	UPROPERTY(EditAnywhere, Category = "AI")
	float HearingHeightMultiplier = 2.0f; // 높이 감지 강화
	
	UFUNCTION()
	void OnHearNoise(const TArray<AActor*>& Actor);
	
	void AttackPlayer();
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	class AActor* SplineActor;
	UPROPERTY(EditAnywhere, Category = "Movement")
 	class USplineComponent* CurrentSpline; // 따라갈 Spline
    
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 200.0f; // 이동 속도

	float DistanceAlongSpline = 0.0f; // 현재 이동 거리
	bool bIsMoving = false;

	class AAIController* AIController;

	
	
};

