// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTriggerBox.h"
#include "EventTriggerBox_Docent.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AEventTriggerBox_Docent : public AEventTriggerBox
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	virtual void BindBeginOverlap() override;

	// 이벤트가 한 번만 실행되도록 제어하는 플래그
	bool bHasTriggered=false;
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UPROPERTY(EditAnywhere, Category = "Docent")
	class ADocent_JumpScare* Docent_JumpScare;
};
