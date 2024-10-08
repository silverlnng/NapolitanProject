// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	int32 ItemInt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UTexture2D* thumnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyData")
	UStaticMesh* ItemMesh;
};

UCLASS()
class NAPOLITANPROJECT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Item)
	TMap<FString,FItemData> ItemMap;
	
};
