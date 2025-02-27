// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_MiddleDoorKey.h"

#include "middleDoor_Fashion.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem_MiddleDoorKey::AItem_MiddleDoorKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem_MiddleDoorKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem_MiddleDoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_MiddleDoorKey::OnPickup()
{
	Super::OnPickup(); //인벤 작업 완료

	//문 찾아서 열 수 있도록 구성
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AmiddleDoor_Fashion::StaticClass());
	if(FoundActor)
	{
		AmiddleDoor_Fashion* MiddleDoor = Cast<AmiddleDoor_Fashion>(FoundActor);
		if(MiddleDoor)
		{
			MiddleDoor->bIsKeyHad = true;
		}
	}
}


