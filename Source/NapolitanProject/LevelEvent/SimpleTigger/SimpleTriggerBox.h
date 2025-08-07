// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleTriggerBox.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASimpleTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleTriggerBox();
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* EditorBillboard;  // 시각화용 컴포넌트
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	class UMyTestGameInstance* GI;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
