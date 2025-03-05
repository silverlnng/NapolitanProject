// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpiderMapGunActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASpiderMapGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpiderMapGunActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	// 나이아가라
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
	// 총구에서 발사

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundWave* FireSoundWave;

	UFUNCTION()
	void Fired();
	
};
