// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ItemHeadActor.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AItemHeadActor : public AItemActor
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	
	virtual void OnPickup() override; // 아이템을 상호작용e키 눌렀을때 부를 함수 + 카메라 앞에 부착하기 

	virtual void PutDown() override;

	UPROPERTY(VisibleAnywhere)
	class ANPC_Cleaner* NPC_Cleaner;

	void CleanerNextState();
	
};
