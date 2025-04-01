// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightControlActor.generated.h"

class ALight;

UCLASS()
class NAPOLITANPROJECT_API ALightControlActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightControlActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	
	UPROPERTY(EditInstanceOnly, Category="Lights")
	TArray<ALight*> ManagedLights;

	// 도슨트 조명은 0번
	
	UPROPERTY()
	TArray<ALight*> SelectedLights;

	UPROPERTY()
	TArray<float> SelectedLightsOriginIntensity;
	
	// 타이머 핸들
	FTimerHandle FlickerTimerHandle;
	
	// 타이머 핸들
	FTimerHandle AutoStopHandle;

	// Flicker 주기
	UPROPERTY(EditAnywhere, Category="Light Flicker")
	float FlickerUpdateRate = 0.02f; // 50fps 정도 주기

	// 사인 파형 설정값
	UPROPERTY(EditAnywhere, Category="Light Flicker")
	float BaseIntensity = 2000.0f;

	UPROPERTY(EditAnywhere, Category="Light Flicker")
	float Amplitude = 400.0f;

	UPROPERTY(EditAnywhere, Category="Light Flicker")
	float FlickerSpeed = 3.0f; // 높을수록 빠르게 깜빡임

	// 함수 선언
	UFUNCTION(BlueprintCallable, Category="Light Flicker")
	void StartSineFlicker(int32 StartIndex, int32 Count,int32 FlickerDuration,float FlickerSpeed_);

	UFUNCTION(BlueprintCallable, Category="Light Flicker")
	void StopSineFlicker();

	void UpdateFlicker();

};
