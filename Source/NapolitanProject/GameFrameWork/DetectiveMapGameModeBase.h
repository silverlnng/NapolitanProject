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
	
	UPROPERTY()
	class UMyTestGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	EDetectiveMapState DetectiveMapState= EDetectiveMapState::FisrtStart;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* FirstEndingSequence;

	UPROPERTY(EditAnywhere, Category = "Cinematic")
	class ULevelSequence* SecondEndingSequence;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> StartLevel;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> HWLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UOpeningWidget> OpeningWidgetFactory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UOpeningWidget* OpeningUI;
	
	UFUNCTION()
	void OpenNextLevel();

	UFUNCTION()
	void OpenHWLevel();

	FTimerHandle UITimer;
	FTimerHandle UITimer2;

	UFUNCTION()
	void ReleasePreloadHandle();
};
