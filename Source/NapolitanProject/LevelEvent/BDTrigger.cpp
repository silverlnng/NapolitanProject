// Fill out your copyright notice in the Description page of Project Settings.


#include "BDTrigger.h"

#include "JumpSkare.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NapolitanProject/GameFrameWork/TestCharacter.h"

// Sets default values
ABDTrigger::ABDTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABDTrigger::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABDTrigger::BeginOverlap);
}

// Called every frame
void ABDTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABDTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Trigger Begin Overlap"));
	if(OtherActor)
	{
		auto* Target = Cast<ATestCharacter>(OtherActor);
		if(Target)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("JumpTag")), FoundActors);

			for (AActor* Actor : FoundActors)
			{
				AJumpSkare* JumpCharacter = Cast<AJumpSkare>(Actor);
				if (JumpCharacter)
				{
					JumpCharacter->JumpSkareStart();
					// 음악 플레이하기
					/*if (LeeSeoSoundWave)
					{
						UGameplayStatics::PlaySound2D(this, LeeSeoSoundWave);
					}*/
					// 바인드 해제
					BoxComponent->OnComponentBeginOverlap.Clear();
					
				}
			}
		}
	}
}

