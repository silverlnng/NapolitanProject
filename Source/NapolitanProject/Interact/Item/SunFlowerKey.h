// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "GameFramework/Actor.h"
#include "SunFlowerKey.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASunFlowerKey : public AItemActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASunFlowerKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup() override; // 아이템을 상호작용e키 눌렀을때 부를 함수 + 카메라 앞에 부착하기 

	virtual void PutDown() override; //e키로 놓기

public:
	bool bIsPickUp = false;

	UPROPERTY(EditDefaultsOnly)
	class AAttachArt* AttachArt;
	

};
