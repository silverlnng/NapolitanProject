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

	UPROPERTY(EditDefaultsOnly)
	class ANPC_Cleaner* Cleaner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	CleanerState animState;

	//몽타주
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* CleaningMontage;

	UFUNCTION()
	void playCleaningMontage();

	UFUNCTION()
	void AnimNotify_FinishClean();

	UFUNCTION()
	void AnimNotify_Decal();
};
