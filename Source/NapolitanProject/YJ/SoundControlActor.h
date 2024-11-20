// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundControlActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASoundControlActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundControlActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp1;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp2;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool IsSecondFloor =false;

	UPROPERTY(VisibleAnywhere)
	class USoundCue* whiteNoise;
	UPROPERTY(VisibleAnywhere)
	class USoundCue* SecondFloor;
	
	UFUNCTION()
	void BGSoundChange(USoundCue* Sound);
	
};
