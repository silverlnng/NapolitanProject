// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBox.h"

#include "Components/BoxComponent.h"
#include "NapolitanProject/GameFrameWork/MyTestGameInstance.h"
#include "NapolitanProject/GameFrameWork/PlayerHUD.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"
#include "NapolitanProject/GameFrameWork/TestPlayerController.h"

// Sets default values
AEventTriggerBox::AEventTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp =CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEventTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TestPC=GetWorld()->GetFirstPlayerController<ATestPlayerController>();
	if (TestPC)
	{
		MainCharacter =TestPC->GetPawn<ATestCharacter>();
		PlayerHUD=TestPC->GetHUD<APlayerHUD>();
	}

	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);

	GI=GetGameInstance<UMyTestGameInstance>();
}

// Called every frame
void AEventTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTriggerBox::BindBeginOverlap()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerBox::BeginOverlap);
}

void AEventTriggerBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}



