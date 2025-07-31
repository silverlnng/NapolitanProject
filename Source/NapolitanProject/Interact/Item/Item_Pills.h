// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Item_Pills.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AItem_Pills : public AItemActor
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ItemID=0;
};
