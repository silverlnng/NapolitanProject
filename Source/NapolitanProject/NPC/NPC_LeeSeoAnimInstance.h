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
	class UAnimMontage* LeeSeo_JumpSkareMontage1;

	UFUNCTION()
	void PlayJumpSkareMontage1();

	UFUNCTION()
	void JumpToSection(const FString& SectionName);

	
	
	
};
