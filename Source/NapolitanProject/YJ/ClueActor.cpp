// Fill out your copyright notice in the Description page of Project Settings.


#include "ClueActor.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AClueActor::AClueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	TextRenderComp=CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComp"));
	TextRenderComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void AClueActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

