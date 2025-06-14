// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapolitanProject/Interface/InteractInterface.h"
#include "ExitDoorTrue.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AExitDoorTrue : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitDoorTrue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ExitDoor; //문

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp; //충돌체

	UPROPERTY(EditAnywhere)
	class USoundBase* OpenDoorSound; //문이 열리는 소리

public:
	class ATestCharacter* MainCharacter;
	
	class ATestPlayerController* TestPC;
	
	UPROPERTY(VisibleAnywhere)
	class APlayerHUD* PlayerHUD;

	UPROPERTY(VisibleAnywhere)
	class UMyTestGameInstance* GI;

	UPROPERTY()
	class USaveGISubsystem* SaveGI;


public:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void DoorOpen();
	// 캐릭터가 상호작용 눌렀을때 실행시킬 함수
	virtual void IInteract() override;

public:
	FTimerHandle TimerHandle;

	virtual void RotateDoor();
	float TargetYaw;
	
	bool bIsOpenDoor = false;
	

};
