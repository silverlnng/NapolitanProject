// Fill out your copyright notice in the Description page of Project Settings.


#include "SunFlowerDoor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
ASunFlowerDoor::ASunFlowerDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	
}

// Called when the game starts or when spawned
void ASunFlowerDoor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASunFlowerDoor::BeginOverlap);
}

// Called every frame
void ASunFlowerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASunFlowerDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		auto* cha=Cast<ATestCharacter>(OtherActor);
		if(cha && bIsOpenKey)
		{
			//레벨이동 -> 큐레이터 방
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,CuratorLevel,true); 
		}
	}
}



