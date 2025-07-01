// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpectorManager.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASpectorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpectorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 현재 점유된 아트 위치들
	UPROPERTY()
	TMap<FVector, ASpector*> OccupiedArtPositions;
    
	// 아트 위치 예약
	UFUNCTION(BlueprintCallable)
	bool ReserveArtPosition(FVector Position, ASpector* Spector);
    
	// 아트 위치 해제
	UFUNCTION(BlueprintCallable)
	void ReleaseArtPosition(FVector Position);
    
	// 사용 가능한 아트 위치 가져오기
	UFUNCTION(BlueprintCallable)
	FVector GetAvailableArtPosition(ASpector* RequestingSpector, const TArray<FVector>& ArtPositions);
    
	// 매니저 인스턴스 가져오기
	UFUNCTION(BlueprintCallable)
	static ASpectorManager* GetInstance(UWorld* World);

public:
	static ASpectorManager* Instance;

};
