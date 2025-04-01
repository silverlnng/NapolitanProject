// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPC_Docent_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPC_Docent_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	// 나의 오너 폰(AEnemy)을 기억하고싶다.
	virtual void NativeInitializeAnimation() override;
	UPROPERTY()
	class ADocent_JumpScare* Docent_JumpScare;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsMoving=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttackStart=false;
};
