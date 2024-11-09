// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"



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

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 ItemID=0;
	
	
//////언리얼 데이터 테이블 읽어오기 	//////////////////////////////////////////////////////////
	
	UPROPERTY(VisibleAnywhere)
	class APlayerController* PC;

	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;

	
	UFUNCTION()
	void OnPickup(); // 아이템을 상호작용e키 눌렀을때 부를 함수

	
	
};
