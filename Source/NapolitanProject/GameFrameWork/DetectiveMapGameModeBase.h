// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DetectiveMapGameModeBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDetectiveMapState:uint8
{
	FisrtStart,
	FirstEnding,
	SecondEnding
};

UCLASS()
class NAPOLITANPROJECT_API ADetectiveMapGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;
	
	class ATestPlayerController* PC;
		
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;

	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	EDetectiveMapState DetectiveMapState= EDetectiveMapState::FisrtStart;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* FirstEndingSequence;
};
