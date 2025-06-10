// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DocentV2.generated.h"

UCLASS()
class NAPOLITANPROJECT_API ADocentV2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADocentV2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	class ATestCharacter* MainCharacter;
	class ATestPlayerController* TestPC;

	class AAIController* AIController;

	class ASoundControlActor* SoundControlActor;
	
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
	/** 몬스터 전용 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* MonsterCamera;
	
	/** 플레이어의 카메라를 몬스터 카메라로 전환하는 함수 */
	void SwitchToMonsterCamera();

	void StopAllTurnDetect();
	
	//3초 뒤돌아있고 ,
	void StartTurnDetect();

	void PlayTurnRightAnimation();

	void PlayTurnOriginAnimation();

///////////////// 플레이어 탐지부분 ////////////////////

	FTimerHandle StartDetectionTimerHandle;
	FTimerHandle StopDetectionTimerHandle;
	FTimerHandle PlayTurnRightAnim;
	FVector LastPlayerLocation;

	UPROPERTY(EditAnywhere)
	float DetectRange=1.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxDetectionDistance = 1800.0f; // 감지 거리 제한 (단위: cm)
	
	bool bCanDetectMovement=false;
	bool InMaxDetectionDistance=false;
	void StartMovementDetection();
	
	void DetectPlayerMovement();

	void StopMovementDetection();
	
//////////////////////////////////////////////////////////////////
	bool bHasDetectedPlayer = false;
	
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;

	void PlayAttackAnimation();
//////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnAroundMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* TurnOriginMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* RunMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* DetectMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundWave* DetectSound;
};
