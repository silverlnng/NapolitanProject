// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CuratorMapGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ACuratorMapGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACuratorMapGameModeBase();

	virtual void BeginPlay() override;

	class ATestPlayerController* PC;
		
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;

	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class ANPCCharacter*> NPCArray;
	
};
