// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpider.generated.h"
UENUM(BlueprintType)
enum class EAttackSpiderState:uint8
{
	Idle,
	MoveToNextPoint,
	Chase,
	Attack
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

	void SetAIState(EAttackSpiderState NewState);

	void StartIdle();
	
	void MoveToNextLocation();
	
	void StartChasing();

	void StartAttack();

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* AttackMontage;
	
	UPROPERTY()
	class UAttackSpider_AnimInstance* Anim;
	
	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);
	
	UPROPERTY(VisibleAnywhere)
	EAttackSpiderState CurrentState= EAttackSpiderState::Idle;
	
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;

	UPROPERTY(VisibleAnywhere,Category = "Navigation")
	TArray<AActor*> TargetPoints;
	
	class AAIController* AIController;

	class UNavigationSystemV1* NavSystem;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	UPROPERTY()
	FTimerHandle IdleTimerHandle;

	UPROPERTY()
	FTimerHandle LostSightTimerHandle;

	void HandleLostSight();
	void CheckAfterAttack();

	UPROPERTY()
	FTimerHandle ChaseCheckTimer;

	void CheckAttackRange();

	void MoveToActor();
	
};
