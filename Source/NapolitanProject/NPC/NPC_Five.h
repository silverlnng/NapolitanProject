// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteract.h"
#include "NPCCharacter.h"
#include "NapolitanProject/Interact/InteractWidget.h"
#include "NPC_Five.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ANPC_Five : public ANPCCharacter,public IIInteract
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	virtual void ResultEvent(int32 result) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =6;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;
	

};
