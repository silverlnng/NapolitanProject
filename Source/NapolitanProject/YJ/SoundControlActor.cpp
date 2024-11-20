// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundControlActor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values
ASoundControlActor::ASoundControlActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	AudioComp1=CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent1"));
	AudioComp1->SetupAttachment(RootComponent);
	
	AudioComp2=CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent2"));
	AudioComp2->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASoundControlActor::BeginPlay()
{
	Super::BeginPlay();
	AudioComp1->Play();
	AudioComp2->Stop();
}

// Called every frame
void ASoundControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundControlActor::BGSoundChange(USoundCue* Sound)
{
	USoundBase* SoundBase_ = Sound;
	AudioComp1->SetSound(SoundBase_);
}

