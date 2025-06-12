// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EyeRoomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API AEyeRoomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AEyeRoomGameModeBase();
	
	virtual void BeginPlay() override;

	class ATestPlayerController* PC;
		
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;

	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class ANPCCharacter*> NPCArray;

	UFUNCTION()
	void Interaction_OnEyeRoomMap(AActor* Interact);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

