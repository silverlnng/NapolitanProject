// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderMapGunActor.h"

#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpiderMapGunActor::ASpiderMapGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);

	
	StaticMeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(BoxComp);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void ASpiderMapGunActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpiderMapGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpiderMapGunActor::Fired()
{
	// 나이아가라 실행
	NiagaraComp->Activate();
	// 조금 흔들림
	
	// 총사운드 실행
	if (FireSoundWave)
	{
		AudioComp->Play();
	}
}

