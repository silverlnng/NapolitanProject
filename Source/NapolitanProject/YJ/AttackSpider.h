// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpider.generated.h"
UENUM(BlueprintType)
enum class EAttackSpiderState:uint8
{
	Idle,
	ChasePlayer,
	Attack,
	Stop
};
UCLASS()
class NAPOLITANPROJECT_API AAttackSpider : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAttackSpider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void MoveAI();                   // AI 이동 처리

	void CheckForPlayer();

	void AttackPlayer();
	
	UPROPERTY(VisibleAnywhere)
	EAttackSpiderState AttackSpiderState= EAttackSpiderState::Idle;
	
	

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRange = 300.0f;
	
	class AAIController* AIController;

	class UNavigationSystemV1* NavSystem;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	
};
