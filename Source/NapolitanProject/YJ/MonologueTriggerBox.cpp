// Fill out your copyright notice in the Description page of Project Settings.


#include "MonologueTriggerBox.h"

#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AMonologueTriggerBox::AMonologueTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMonologueTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
	}
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&AMonologueTriggerBox::BoxCompEndOverlap);
}

// Called every frame
void AMonologueTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonologueTriggerBox::BoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// MainCharacter 의 MonologueUI 에 나오도록 해야함 .
	// 각자의 Monologue_ID 에 따라서 다른 글을 나오도록 하기 
}

void AMonologueTriggerBox::BoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

