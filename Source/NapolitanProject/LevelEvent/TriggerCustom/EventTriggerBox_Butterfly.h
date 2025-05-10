// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTriggerBox.h"
#include "EventTriggerBox_Butterfly.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AEventTriggerBox_Butterfly : public AEventTriggerBox
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool bHasTriggered=false;

	UPROPERTY()
	class APicture_Zombie* Picture_Zombie;
};
