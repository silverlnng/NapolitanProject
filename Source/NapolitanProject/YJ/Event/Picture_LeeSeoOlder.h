// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/LevelEvent/JumpScare_Picture.h"
#include "Picture_LeeSeoOlder.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APicture_LeeSeoOlder : public AJumpScare_Picture
{
	GENERATED_BODY()

	APicture_LeeSeoOlder();

	virtual void BeginPlay() override;
	
	virtual void PlayBasicAnimMontage() override;

	virtual void PlaySittingAnimMontage()override;
	
	virtual void PlayPaintAnimMontage()override;
	
	virtual void PaintAnimEnd() override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* parentMeshComponent1;
	
	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* DefeatAnimMontage;
};
