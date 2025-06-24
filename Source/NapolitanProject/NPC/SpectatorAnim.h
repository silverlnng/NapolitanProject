// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spector.h"
#include "Animation/AnimInstance.h"
#include "SpectatorAnim.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API USpectatorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class ASpector* Spectator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="State");
	ESpectorState animState;
	
	
};
