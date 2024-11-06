// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC_Security.h"
#include "Animation/AnimInstance.h"
#include "NPC_Security_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPC_Security_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 나의 오너 폰(AEnemy)을 기억하고싶다.
	virtual void NativeInitializeAnimation() override;

	UPROPERTY()
	class ANPC_Security* NPCSecurity;

	// NPC_Security 의 state 와 일치시켜주기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ESecurityState State;
	
};
