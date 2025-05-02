// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTriggerBox.h"
#include "EventTriggerBox_LobbyRoom.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AEventTriggerBox_LobbyRoom : public AEventTriggerBox
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	// 이벤트가 한 번만 실행되도록 제어하는 플래그
	bool bHasTriggered=false;
	class ASoundControlActor* SoundControlActor;
	
	virtual void BindBeginOverlap();
	
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	virtual void BindEndOverlap();
	
	
	virtual void EndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
