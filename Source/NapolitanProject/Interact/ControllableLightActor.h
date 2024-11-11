// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControllableLightActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AControllableLightActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AControllableLightActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	// 버튼 메쉬 와 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* BtnMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;
	
	// 라이트 몇개 부착
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp0;
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp1;
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp2;
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp3;
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp4;
	UPROPERTY(EditDefaultsOnly,Category=Light)
	class URectLightComponent* RectLightComp5;

	UPROPERTY(EditDefaultsOnly,Category=Light)
	UChildActorComponent* BP_Drop_Ceiling_01;

	UPROPERTY(EditDefaultsOnly,Category=Light)
	UChildActorComponent* BP_Drop_Ceiling_02;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Light)
	float RectLightIntensity = 2000.f;
	
	UPROPERTY(VisibleAnywhere)
	TArray<URectLightComponent*> RectLightArray;
	
	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> AllChildren;
	
	UPROPERTY(VisibleAnywhere)
	TArray<URectLightComponent*> Drop_CeilingRectLightArray;
	
	UPROPERTY(VisibleAnywhere)
	bool IsTurnOn=false;

	UFUNCTION()
	void BtnMeshOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void TurnOnLight(bool value); // 한개 함수로 여러 light 제어 해야함
	

};

