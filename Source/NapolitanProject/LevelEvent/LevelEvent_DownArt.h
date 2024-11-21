// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEvent_DownArt.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ALevelEvent_DownArt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelEvent_DownArt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* Frame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* Box;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float LerpAlpha = 0.0f; // 초기 Lerp 알파값

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TargetZ")
	float TargetZ = -110.f; // 목표 Z 위치
	
	float MoveSpeed = 4.0f;  // 이동 속도 (값을 조정 가능)

	FVector StartLocation;  // 시작 위치
	FVector TargetLocation; // 목표 위치
	FTimerHandle LerpTimerHandle; // 타이머 핸들

	void MoveSmoothly(); //아래로 내려가는거
	void MoveUp(); //다시 위로 올라가는거
	void MoveSmoothlyUp();

	UPROPERTY(EditAnywhere)
	class USoundBase* ScreamSound; //웃음소리 또는 비명소리 
	
};
