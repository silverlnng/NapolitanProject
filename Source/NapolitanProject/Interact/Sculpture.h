// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sculpture.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ASculpture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASculpture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	//  메쉬 와 콜리전
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp1;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp2;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp3;

	// 아이템을 가진상태에서 아이템을 내려놓을수있도록 
	void PutDownPiece(class APieceActor* Piece);

	UPROPERTY(VisibleAnywhere)
	TArray<APieceActor*> PieceArray;

	// 아이템 3개가 모두 모였는지 확인하는 함수
	void MissionCheck();
	
};
