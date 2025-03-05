// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSpiderV2.h"

#include "AIController.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAttackSpiderV2::AAttackSpiderV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void AAttackSpiderV2::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	
}

// Called every frame
void AAttackSpiderV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttackSpiderV2::MoveAlongSpline(float DeltaTime)
{
}

void AAttackSpiderV2::StartMoving()
{
}


