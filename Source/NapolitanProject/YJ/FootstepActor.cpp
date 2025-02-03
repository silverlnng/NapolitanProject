// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepActor.h"

#include "Components/DecalComponent.h"

// Sets default values
AFootstepActor::AFootstepActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Decal 컴포넌트 추가
	FootstepDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("FootstepDecal"));
	RootComponent = FootstepDecal;

	// 기본 크기 설정
	FootstepDecal->DecalSize = FVector(20.0f, 20.0f, 5.0f);
	FootstepDecal->SetFadeScreenSize(0.01f);
}

// Called when the game starts or when spawned
void AFootstepActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFootstepActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

