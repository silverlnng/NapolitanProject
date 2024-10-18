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
	void ResultEvent(int32 result) override;
	
};
