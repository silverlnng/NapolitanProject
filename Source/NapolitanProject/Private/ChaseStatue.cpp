// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseStatue.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AChaseStatue::AChaseStatue()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	CSCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CSCol"));
	CSCol->SetCapsuleHalfHeight(90.f);
	CSCol->SetCapsuleRadius(90.f);

}

// Called when the game starts or when spawned
void AChaseStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChaseStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChaseStatue::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

