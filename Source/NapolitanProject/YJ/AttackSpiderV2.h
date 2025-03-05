// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpiderV2.generated.h"

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


	
private:
	UPROPERTY(EditAnywhere, Category = "Movement")
 	class USplineComponent* CurrentSpline; // 따라갈 Spline
    
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 200.0f; // 이동 속도

	float DistanceAlongSpline = 0.0f; // 현재 이동 거리
	bool bIsMoving = false;

	class AAIController* AIController;

	
	
};
