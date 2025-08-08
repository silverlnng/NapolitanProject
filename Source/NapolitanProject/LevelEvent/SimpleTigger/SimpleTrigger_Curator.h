// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleTriggerBox.h"
#include "SimpleTrigger_Curator.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ASimpleTrigger_Curator : public ASimpleTriggerBox
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	// 큐레이터를 만난후 추격이벤트에서 발생시킬 트리거 
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere,Category=JumpScare)
	class ACharacter* ZombieMutant;
	UPROPERTY(EditAnywhere,Category=JumpScare)
	class UAnimMontage* attackAM;
};
