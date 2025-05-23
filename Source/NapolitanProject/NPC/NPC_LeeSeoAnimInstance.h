// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPC_LeeSeoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UNPC_LeeSeoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	
	UPROPERTY(EditDefaultsOnly)
	class ANPC_LeeSeo* LeeSeo;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* LeeSeo_JumpSkareMontage1; //뒤로 도는 몽타주

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* LeeSeo_JumpSkareMontage2; // 달려오는 몽타주

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* LeeSeo_JumpSkareMontage3; //칼로 찌르는 몽타주

	UFUNCTION()
	void PlayJumpSkareMontage1();

	UFUNCTION()
	void PlayJumpSkareMontage2();

	UFUNCTION()
	void PlayJumpSkareMontage3();

	UFUNCTION()
	void AnimNotify_SwitchCamera();
	
	
};
