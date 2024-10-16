// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	///Script/Engine.DataTable'/Game/YJ/Item/DT_Item.DT_Item'
	
	UDataTable* DT_itemData =LoadObject<UDataTable>(nullptr,TEXT("'/Game/YJ/Item/DT_Item.DT_Item'"));

	//먼저 모든 행단위로 가져오기  
	TArray<FName> RowNames = DT_itemData->GetRowNames();
	
	for (int i = 0; i < RowNames.Num(); i++)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*RowNames[i].ToString());
	}
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

