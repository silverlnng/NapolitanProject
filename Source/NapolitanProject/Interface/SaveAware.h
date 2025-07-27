// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveAware.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveAware : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NAPOLITANPROJECT_API ISaveAware
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 각 액터는 자신이 어떤 ID로 식별되는지를 알려줘야 함
	virtual int32 GetAssociatedNpcID() const = 0;
};
