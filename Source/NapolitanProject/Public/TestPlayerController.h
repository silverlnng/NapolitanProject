// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

/**
 * 
 */



UCLASS()
class NAPOLITANPROJECT_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere)
	class UTestGameInstance* GI;
	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;
	UPROPERTY(VisibleAnywhere)
	class USouvenirWidget* SouvenirUI;
	
	// SouvenirUI
	UFUNCTION()
	void SetSouvenirUICurNumber(int curNum);
};
