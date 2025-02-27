// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelMoveDoor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ALevelMoveDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelMoveDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	//UPROPERTY(EditDefaultsOnly)
	//class UBoxComponent* BoxComp2;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* FromLevelLocComp;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	class UMyTestGameInstance* GI;

	// 위치
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LevelMove();
	
	UFUNCTION()
	void BeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 출력할 독백을 배열로 정의해두기
	UPROPERTY(EditAnywhere)
	TArray<FString> TextLines;
public:
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> MoveToLevel;

	UPROPERTY(EditAnywhere)
	bool bMoveFromLobby=false;
	
};
