// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ATriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	class ASoundControlActor* SoundControlActor;
	
	UPROPERTY(VisibleAnywhere)
	class ANPC_Security* NPC_Security;

	UPROPERTY(VisibleAnywhere)
	TArray<class AControllableLightActor*> ControllableLightArray;

	UFUNCTION()
	void CheckSide();

	UFUNCTION()
	void BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxCompEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};

