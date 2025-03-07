// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackSpiderV2.generated.h"

class UCameraComponent;

UENUM(BlueprintType)
enum class EAttackSpiderV2State:uint8
{
	Move,
	Drop,
	Chase,
	Attack
};

UCLASS()
class NAPOLITANPROJECT_API AAttackSpiderV2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAttackSpiderV2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY()
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;
	class APlayerHUD* PlayerHUD;
	
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp2;
	
public:
	void MoveAlongSpline(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* AIPerception;
	
	void AttackPlayer();

	void DetectAndDrop();

	void StartChasing();

	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void CheckAttackRange();

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	class AActor* SplineActor;
	UPROPERTY(EditAnywhere, Category = "Movement")
 	class USplineComponent* CurrentSpline; // 따라갈 Spline

public:
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 50.0f; // 이동 속도

	float DistanceAlongSpline = 0.0f; // 현재 이동 거리
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadWrite)
	float toPlayerDistance;

	
	
	class AAIController* AIController;

	
	UPROPERTY()
	class UAttackSpider_AnimInstance* Anim;
	
	UPROPERTY(VisibleAnywhere)
	EAttackSpiderV2State CurrentState= EAttackSpiderV2State::Move;
	
	UFUNCTION()
	void SetAIState(EAttackSpiderV2State NewState);

	UPROPERTY(EditAnywhere)
	float SoundControlDistance=1000;
	
	class ASoundControlActor* SoundControlActor;

	bool bIsFadingOut = false;
	bool bIsFadingIn = false;
	bool bAttack = false;
	
	UFUNCTION()
	void SoundControl();
	
	
	UPROPERTY()
	FTimerHandle ChaseCheckTimer;
	
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	/** 몬스터 전용 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* MonsterCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	float FadeTime=7.f;
	/** 플레이어의 카메라를 몬스터 카메라로 전환하는 함수 */
	void SwitchToMonsterCamera();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* AttackSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* DetectSound;
	
};

