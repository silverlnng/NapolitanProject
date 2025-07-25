// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExitDoorTrue.h"
#include "ExitDoor_First.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AExitDoor_First : public AExitDoorTrue
{
	GENERATED_BODY()
	
public:
	
	void BindBeginOverlap();
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void RotateDoor() override;

	void EndEvent();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UNormalGameEndingWidget> GameEndingwidget;

	UNormalGameEndingWidget* GameEndingUI;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> StartLevel;
	
	//////////////////소리///////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* EndEventSoundWave;
};
