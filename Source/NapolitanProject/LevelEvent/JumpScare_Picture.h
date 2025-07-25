// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JumpScare_Picture.generated.h"

UCLASS()
class NAPOLITANPROJECT_API AJumpScare_Picture : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJumpScare_Picture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;
	
	/** 몬스터 전용 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* MonsterCamera;

	// 붓 메쉬를 소켓에 부착시키기
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* AttachedStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PaintingCanvasStaticMeshComponent;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MovetoForward();

	void StopInitialForwardMovement();
	void StartChasingTarget();
	void MoveTowardTarget(float DeltaTime);
	
	UPROPERTY()
	FTimerHandle ChaseCheckTimer;
	
	void CheckAttackRange();

	void StartAttack();
	
	UPROPERTY(EditDefaultsOnly)
	class USoundWave* AttackSound;
	
	bool bAttack = false;

	UFUNCTION(BlueprintCallable)
	virtual void PlayBasicAnimMontage();

	UFUNCTION(BlueprintCallable)
	virtual void PlaySittingAnimMontage();
	
	UFUNCTION(BlueprintCallable)
	virtual void PlayPaintAnimMontage();

	UFUNCTION(BlueprintCallable)
	virtual void PaintAnimEnd();
	
	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* PaintAnimMontage;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintImplementableEventPlayPaintAnim();
	
	void PlayRunAnimMontage();

	void PlayJumpAttackAnimMontage();
	
	/** 플레이어의 카메라를 몬스터 카메라로 전환하는 함수 */
	void SwitchToMonsterCamera();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
	
	UPROPERTY(VisibleAnywhere)
	float toPlayerDistance;
	UPROPERTY(EditAnywhere)
	float AttackRange = 300.0f;

	UPROPERTY(EditAnywhere)
	float InitialMoveDuration=5.0f;
	
    UPROPERTY(EditAnywhere)
	float MoveSpeed = 5.0f;
	UPROPERTY(EditAnywhere)
	float MoveTowardTargetSpeed = 5.0f;
	UPROPERTY(EditAnywhere)
	float SwitchToMonsterCameraDelay = 2.5f;

	UAnimInstance* AnimInstance;
	
	bool bIsMovingForward = false;
private:
	FVector InitialMoveDirection;
	bool bIsChasing = false;
	bool bAttackRangeIn=false;
	FTimerHandle StopMoveTimerHandle;

	class ATestPlayerController* TestPC;
	UPROPERTY(EditAnywhere, Category = "Target")
	class ATestCharacter* TargetCharacter; // 플레이어 캐릭터

	

	// 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* attackAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* basicAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* JumpAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category=Anim)
	class UAnimMontage* SitAnimMontage;
	
};
