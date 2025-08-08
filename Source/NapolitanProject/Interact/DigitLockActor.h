// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DigitLockActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ADigitLockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADigitLockActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	bool isInUi;
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* DigitLockUi;

	// 메인플레이어의 인터렉트 UI 끄기.
	class ATestCharacter* MainCharacter;
	
	class ATestPlayerController* TestPC;

	class APlayerHUD* PlayerHUD;
	
	UFUNCTION(BlueprintCallable)
	void SetVisisbleInteractUI();
	
};
