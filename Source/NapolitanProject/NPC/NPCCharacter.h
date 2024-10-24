// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 NPC_ID =-1;

	int32 State =-1;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	virtual int32 GetNPCID();

	virtual int32 GetState(); 
	
	virtual void ResultEvent(int32 result); //결과 함수

	virtual void Interact(); 

	// 전환용 카메라 부착해주기 
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
};

