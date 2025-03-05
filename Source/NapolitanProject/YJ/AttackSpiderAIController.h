// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AttackSpiderAIController.generated.h"

struct FAIStimulus;
/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AAttackSpiderAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	AAttackSpiderAIController();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerception;
private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Hearing* HearingConfig;

public:
	UFUNCTION()
	void OnHearNoise(const TArray<AActor*>& Actor);
	
	UPROPERTY()
	class AAttackSpiderV2* AttackSpider;
};
