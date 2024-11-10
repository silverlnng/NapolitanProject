// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SouvenirActor.h"
#include "Souvenir_Dongjun.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASouvenir_Dongjun : public ASouvenirActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASouvenir_Dongjun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
