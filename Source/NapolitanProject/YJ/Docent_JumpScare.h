// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Docent_JumpScare.generated.h"

class UCameraComponent;

UCLASS()
class NAPOLITANPROJECT_API ADocent_JumpScare : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADocent_JumpScare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
	UPROPERTY()
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY()
	class UNPC_Docent_AnimInstance* Anim;

	UPROPERTY(EditAnywhere, Category = "Movement")
	class AActor* SplineActor;
	UPROPERTY(EditAnywhere, Category = "Movement")
	class USplineComponent* CurrentSpline; // 따라갈 Spline
	
	void MoveAlongSpline(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SplineMoveSpeed = 50.0f; // 이동 속도
	float DistanceAlongSpline = 0.0f; // 현재 이동 거리
	bool bIsMoving = false;
	
	/** 몬스터 전용 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* MonsterCamera;
	
	/** 플레이어의 카메라를 몬스터 카메라로 전환하는 함수 */
	void SwitchToMonsterCamera();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	void StartAttack();
	bool bAttack = false;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* AttackSound;

	UPROPERTY(EditAnywhere)
	FString description=FString(TEXT("도슨트에게 먹혔다"));
};
