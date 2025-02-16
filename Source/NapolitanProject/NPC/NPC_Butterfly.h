// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "NPC_Butterfly.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ANPC_Butterfly : public ANPCCharacter
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void Interact() override;

	virtual void ResultEvent(int32 result) override; 
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =8;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;

	UPROPERTY()
	class ATargetForItem_BurgerPlate* TargetForItem_BurgerPlate;
		
	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<class AItemActor> CutterItemClass;
	UFUNCTION()
	void SpawnItems();
};
