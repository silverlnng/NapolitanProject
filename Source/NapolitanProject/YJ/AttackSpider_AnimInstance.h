// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackSpider.h"
#include "AttackSpiderV2.h"
#include "Animation/AnimInstance.h"
#include "AttackSpider_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UAttackSpider_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 나의 오너 폰(AEnemy)을 기억하고싶다.
	virtual void NativeInitializeAnimation() override;
	UPROPERTY()
	class AAttackSpiderV2* AttackSpider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAttackSpiderV2State CurrentState;

	UFUNCTION()
	void AnimNotify_CheckAfterAttack();

	UFUNCTION()
	void AnimNotify_MoveToActor();
	
	UFUNCTION()
	void AnimNotify_AttackMiddle();

	UFUNCTION(BlueprintCallable)
	void SetState(EAttackSpiderV2State State);
};
