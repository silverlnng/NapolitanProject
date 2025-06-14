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
	class USaveGISubsystem* SaveGI;
	// 위치
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

	// 설치한 위치가 로비(메인레벨)면 true 설정을 , 다른레벨이면 false를 설정
	UPROPERTY(EditAnywhere)
	bool bMoveFromLobby=false;

	// 로드해야하는 서브레벨들을 블프에서 할당하기 
	UPROPERTY(EditDefaultsOnly)
	TSet<TSoftObjectPtr<UWorld>> SubLevelArray;
	
	/*UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> CorriderLevel;
	
	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyRoom1Level;

	UPROPERTY(EditDefaultsOnly)
	const TSoftObjectPtr<UWorld> LobbyRoom2Level;*/
	
};
