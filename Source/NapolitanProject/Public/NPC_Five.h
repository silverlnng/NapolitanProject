// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "NPC_Five.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ANPC_Five : public ANPCCharacter
{
	GENERATED_BODY()

public:
	virtual void ResultEvent(int32 result) override;
	
	int32 NPC_ID =5;
	
	int32 State =0;

	
	
};
