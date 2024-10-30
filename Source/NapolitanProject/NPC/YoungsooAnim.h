// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "YoungsooAnim.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API UYoungsooAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* YSSadMontage; // 1-0. 괜찮으십니까 애니메이션

	//몽타주 실행
	UFUNCTION()
	void playYSSadMontage();
	
};
