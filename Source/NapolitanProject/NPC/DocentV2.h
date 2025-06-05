// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DocentV2.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ADocentV2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADocentV2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	

	//3초 뒤돌아있고 ,
	void ChoseRandomTimeTurnRightAnim();

	void PlayTurnRightAnimation();

	void PlayTurnOriginAnimation();

///////////////// 플레이어 탐지부분 ////////////////////

	FTimerHandle StartDetectionTimerHandle;
	
	FVector LastPlayerLocation;

	UPROPERTY(EditAnywhere)
	float DetectRange=3.0f;
	
	bool bCanDetectMovement=false;
	
	void StartMovementDetection();
	
	void DetectPlayerMovement();

	void StopMovementDetection();
	
//////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnAroundMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnOriginMontage;
	
};
