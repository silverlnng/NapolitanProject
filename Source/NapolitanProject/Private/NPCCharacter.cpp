// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("NPC"));
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacter::ResultEvent(int32 result)
{
	// result에 따라서 경우의 수 나누기 
}

void ANPCCharacter::Interact()
{
}

