// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightHouse.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ALightHouse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightHouse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* LightRotationRoot; //회전할 루트 씬 컴포넌트 (빛의 회전 중심)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpotLightComponent* SpotLight; //스포트라이트 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* LightHouseMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float RotationSpeed; //회전 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float LightIntensity; //빛의 강도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	FLinearColor LightColor; //빛의 색상

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float SpotLightAngle; //스포트라이트 각도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float LightDistance; //빛의 도달 거리

	// 빛 깜빡임 효과 활성화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	bool bEnableBlinking;

	// 깜빡임 주기 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float BlinkingInterval;
	
	// 현재 회전 각도
	float CurrentRotation;

	// 깜빡임 타이머
	float BlinkingTimer;

	// 현재 빛이 켜져있는지 상태
	bool bIsLightOn;

public:
	// 빛의 회전 처리
	void RotateLight(float DeltaTime);

	// 빛 깜빡임 처리
	void HandleBlinking(float DeltaTime);
};
