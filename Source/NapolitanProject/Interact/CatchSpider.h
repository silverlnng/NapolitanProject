// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CatchSpider.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ACatchSpider : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACatchSpider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 네비게이션을 이용해서 길찾기를 하고싶다.
	UPROPERTY(VisibleAnywhere)
	class AAIController* EnemyAI;
	
	UPROPERTY()
	class UCatchSpider_AnimInstance* Anim;
	
	FVector PatrolPoint;
	float PatrolPointRadius = 700;

	bool SetPatrolPoint(FVector origin, float radius, FVector& dest);

	int32 Health =3;
	
	void Damaged();

	void Die();
	
	UPROPERTY(VisibleAnywhere)
	bool Died=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ItemID=5;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
};
