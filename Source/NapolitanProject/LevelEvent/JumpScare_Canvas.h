// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpScare_Canvas.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AJumpScare_Canvas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpScare_Canvas();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CanvasStaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* SceneCaptureComponent2D;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp; //충돌체

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 이벤트가 한 번만 실행되도록 제어하는 플래그
	bool bHasTriggered=false;
	
	void StartJumpScare();

	UPROPERTY(EditInstanceOnly)
	class AJumpScare_Picture* JumpScarePicture_Character;
};

