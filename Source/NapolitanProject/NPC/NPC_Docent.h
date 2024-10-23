// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "NPC_Docent.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ANPC_Docent : public ANPCCharacter
{
	GENERATED_BODY()
public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =2;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;
	
};
