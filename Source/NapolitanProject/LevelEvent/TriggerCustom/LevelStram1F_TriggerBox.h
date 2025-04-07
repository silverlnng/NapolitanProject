// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTriggerBox.h"
#include "LevelStram1F_TriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ALevelStram1F_TriggerBox : public AEventTriggerBox
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	virtual void BindBeginOverlap() override;
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	// 이벤트가 한 번만 실행되도록 제어하는 플래그
	bool bHasTriggered=false;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyLevel;
};
