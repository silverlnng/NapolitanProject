// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SouvenirActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASouvenirActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASouvenirActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 SouvenirID=0;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	//  메쉬 와 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SouvenirName= FString(TEXT(""));

	virtual int32 GetSouvenirID();

	virtual FString GetSouvenirName();
	
	virtual void OnPickup();


};

