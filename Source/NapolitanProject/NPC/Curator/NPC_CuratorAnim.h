// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaseStatue.h"
#include "Animation/AnimInstance.h"
#include "NPC_CuratorAnim.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPC_CuratorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class AChaseStatue* Curator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	ChaseStatueState animState;

	UFUNCTION()
	void PlayWalkMontage();

	//몽타주
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* WalkMontage;
	
};
