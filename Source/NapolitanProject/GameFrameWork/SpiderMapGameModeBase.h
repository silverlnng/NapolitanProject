// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpiderMapGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ASpiderMapGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASpiderMapGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY()
	class UMyTestGameInstance* GI;
	
	class ATestPlayerController* PC;
	
	class ATestCharacter* MainCharacter;
	
	class APlayerHUD* PlayerHUD;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	int32 CatchSpiderCount=0;
	UPROPERTY(EditAnywhere)
	int32 SpiderItemID=3;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AItemActor> SpiderBP;

	UPROPERTY()
	class AItemActor* SpiderItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASpiderMapGunActor> GunBP;

	UPROPERTY()
	class ASpiderMapGunActor* Gun;
	
	UFUNCTION()
	void Interaction_OnSpiderMap(AActor* Interact);

	UFUNCTION()
	void MakeNoisePlayer();

	UPROPERTY(EditAnywhere)
	float NoiseRange=10;

	UPROPERTY(EditAnywhere)
	float AddZ=600;

	UPROPERTY()
	class AAttackSpiderV2* AttackSpiderV2;
};


