// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClueActor.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AClueActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClueActor();

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
	class UBoxComponent* BoxComp;

	// 단서마다 각자의 고유한 내용이 있음
	UPROPERTY(EditDefaultsOnly)
	FString ClueContents;

	UPROPERTY(EditDefaultsOnly)
	class UTextRenderComponent* TextRenderComp;
    // 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	void LookAt();
};


