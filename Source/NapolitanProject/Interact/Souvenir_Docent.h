// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SouvenirActor.h"
#include "Souvenir_Docent.generated.h"

/**
 * 
 */
UCLASS()
class NAPOLITANPROJECT_API ASouvenir_Docent : public ASouvenirActor
{
	GENERATED_BODY()
	
	int32 SouvenirID=2;
	
	FString SouvenirName= FString(TEXT("수첩"));
	
	virtual int32 GetSouvenirID();

	virtual FString GetSouvenirName();
	
	virtual void OnPickup();
	
};
