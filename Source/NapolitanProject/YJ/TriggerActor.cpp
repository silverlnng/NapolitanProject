// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerActor.h"

#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"
#include "NapolitanProject/NPC/NPC_Security.h"

// Sets default values
ATriggerActor::ATriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ANPC_Security> It(GetWorld(), ANPC_Security::StaticClass()); It; ++It)
	{
		NPC_Security = *It;
	}
	TestPC = GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	MainCharacter =TestPC->GetPawn<ATestCharacter>();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ATriggerActor::BoxCompOverlap);
}

// Called every frame
void ATriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerActor::BoxCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
