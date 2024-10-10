// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UInventoryWidget> InventoryFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UInventoryWidget* InventoryUI;

	
	
};
