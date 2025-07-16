// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartLevelGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AStartLevelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	class UMyTestGameInstance* GI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> OpeningWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UUserWidget* OpeningUI;
};
