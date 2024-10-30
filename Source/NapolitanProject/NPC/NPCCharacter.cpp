// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TestPlayerController.h"
#include "NapolitanProject/YJ/PlayerHUD.h"
#include "NapolitanProject/YJ/TestCharacter.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("NPC"));
	// 스프링암을 생성해서 루트에 붙이고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	//SpringArmComp->SetupAttachment(GetMesh(),"head");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0 , 0 , 30));
	SpringArmComp->SetRelativeRotation(FRotator(0,180,0));
	SpringArmComp->TargetArmLength = 300;

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	 TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	 MainCharacter =TestPC->GetPawn<ATestCharacter>();
	 PlayerHUD =TestPC->GetHUD<APlayerHUD>();
	//TestPC00
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

int32 ANPCCharacter::GetNPCID()
{
	return NPC_ID;
}

int32 ANPCCharacter::GetState()
{
	return State;
}

void ANPCCharacter::ResultEvent(int32 result)
{
	// result에 따라서 경우의 수 나누기 
}

void ANPCCharacter::Interact()
{
}

void ANPCCharacter::playTalkAnimMontage()
{
	
}

