// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Picture_Zombie.generated.h"

UCLASS()
class NAPOLITANPROJECT_API APicture_Zombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APicture_Zombie();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp1;
	
	void DropEffect();

	
};


