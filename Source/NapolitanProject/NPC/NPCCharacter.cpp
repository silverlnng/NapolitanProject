// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "../GameFrameWork/MyTestGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

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
	 GI = GetGameInstance<UMyTestGameInstance>();

	//TestPC00
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bisDissolve && DynamicMaterial)
	{
		dissolveAnimValue += DeltaTime / 4;

		// 원하는 범위 (0.5에서 -0.5)로 클램핑
		float DissolveValue = FMath::Clamp(0.5f - dissolveAnimValue, -0.5f, 0.5f);
		DynamicMaterial->SetScalarParameterValue(TEXT("dissolve"), DissolveValue);

		if (DissolveValue <= -0.5f)
		{
			bisDissolve = false;
			GetMesh()->SetVisibility(false);
		}
	}

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

void ANPCCharacter::DissolveEvent(FString& str)
{
	if (str=="Yellow")
	{
		if (DissolveMaterial_Yellow)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial_Yellow, this);
			if (DynamicMaterial)
			{
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
		}
	}
	else if (str=="Black")
	{
		if (DissolveMaterial_Black)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(DissolveMaterial_Black, this);
			if (DynamicMaterial)
			{
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
		}
	}

	bisDissolve = true; // tick 작동하도록
	IsCleared=true;
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
	//ChangeCleared(); // 상호작용 e키 안나오도록 
}

void ANPCCharacter::ChangeCleared()
{
	 // 콜리전 프로필을 다른거로 변경
	IsCleared=true;
	GetComponentByClass<UCapsuleComponent>()->SetCollisionProfileName(FName("ClearedNPC"));
	GI->ClearedNPC.Add(GetNPCID());
	FString id =FString::FromInt(GetNPCID());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *id);
}

