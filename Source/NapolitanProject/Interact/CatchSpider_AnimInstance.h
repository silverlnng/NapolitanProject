// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CatchSpider_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UCatchSpider_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* DamagedMontage;

	UFUNCTION()
	void PlayDamagedMontage();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDied=false;
};
