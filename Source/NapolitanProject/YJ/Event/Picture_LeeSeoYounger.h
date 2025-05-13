// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/LevelEvent/JumpScare_Picture.h"
#include "Picture_LeeSeoYounger.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APicture_LeeSeoYounger : public AJumpScare_Picture
{
	GENERATED_BODY()

	APicture_LeeSeoYounger();

	virtual void BeginPlay() override;
	
	virtual void PlayBasicAnimMontage() override;
	
	virtual void PlaySittingAnimMontage()override;
	
	virtual void PlayPaintAnimMontage()override;
	
	virtual void PaintAnimEnd() override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* parentMeshComponent1;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* parentMeshComponent2;

	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* ClapAnimMontage;
};
