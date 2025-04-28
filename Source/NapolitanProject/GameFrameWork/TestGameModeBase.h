// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NapolitanProject/YJ/Save/TestSaveGame.h"
#include "TestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameModeBase();

	virtual void StartPlay() override;
	
	virtual void BeginPlay() override;

	
	class ATestPlayerController* PC;
		
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;

	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class ANPCCharacter*> NPCArray;

	UPROPERTY(VisibleAnywhere)
	TMap<int32,class AItemActor*> ItemActorArray;
	UPROPERTY(EditInstanceOnly)
	class AItemActor* BreadItem4;
	UPROPERTY(EditInstanceOnly)
	class AItemActor* BreadItem5;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> CorriderLevel;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyRoom1Level;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyRoom2Level;
private:
	
};
