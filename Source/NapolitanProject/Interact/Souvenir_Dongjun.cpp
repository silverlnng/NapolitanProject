// Fill out your copyright notice in the Description page of Project Settings.


#include "Souvenir_Dongjun.h"

// Sets default values
ASouvenir_Dongjun::ASouvenir_Dongjun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASouvenir_Dongjun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASouvenir_Dongjun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ASouvenir_Dongjun::GetSouvenirID()
{
	return this->SouvenirID;
}

FString ASouvenir_Dongjun::GetSouvenirName()
{
	return this->SouvenirName;
}

void ASouvenir_Dongjun::OnPickup()
{
	Super::OnPickup();
}

