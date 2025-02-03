// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepSpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAPOLITANPROJECT_API UFootstepSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepSpawnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 발자국을 생성하는 함수
	UFUNCTION(BlueprintCallable)
	void SpawnFootsteps(const FVector& Destination);

	// 발자국을 생성할 액터 클래스 (Blueprint에서 설정 가능)
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<class AFootstepActor> FootstepActorClass;

	// 발자국 간격
	UPROPERTY(EditAnywhere,Category = "Settings")
	float StepDistance = 100.0f;
		
};
