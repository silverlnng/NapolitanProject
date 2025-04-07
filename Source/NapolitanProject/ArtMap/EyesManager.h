// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EyesManager.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AEyesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEyesManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category="Gimmick|Area")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category="Gimmick|Config")
	float TotalTime = 60.0f; // 4분, 제한시간

public:
	UPROPERTY()
	TArray<class AOriginEye*> OriginEyes;

	float TimeElapsed;

	void UpdateEyeVisibility(); //보이게 하도록 하는 함수
	void OnAllEyesRevealed(); //만약 시간 내에 기믹을 풀지 못했다면 사망


};
