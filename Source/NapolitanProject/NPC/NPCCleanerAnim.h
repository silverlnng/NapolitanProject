// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPC_Cleaner.h"
#include "NPCCleanerAnim.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPCCleanerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	CleanerState animState;

	UPROPERTY(EditDefaultsOnly)
	class ANPC_Cleaner* Cleaner;

	
};
