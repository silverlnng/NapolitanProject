// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpider.generated.h"

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
	
	void CheckCeilingAndGround();   // 천장 & 지상 감지
	void SwitchToCeilingMode();     // 천장 모드 전환
	void SwitchToGroundMode();      // 지상 모드 전환
	void MoveAI();                   // AI 이동 처리

	void CheckForPlayer();

	void AttackPlayer();

	void ReturnToCeiling();
	
	UPROPERTY(EditAnywhere)
	bool bIsOnCeiling = true;       // 현재 천장 모드 여부

	UPROPERTY(EditAnywhere)
	float TraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRange = 300.0f;
	
	class AAIController* AIController;

	class UNavigationSystemV1* NavSystem;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	
};
