// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "NPCDongjon.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ANPCDongjon : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCDongjon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =3; //고뇌하는 노인의 번호

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

};
