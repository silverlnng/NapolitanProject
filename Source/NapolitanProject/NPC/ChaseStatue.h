// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "ChaseStatue.generated.h"

UENUM(BlueprintType)
enum class ChaseStatueState : uint8 {
	Patrol,
	Move,
	Attack,
};

UCLASS()
class NAPOLITANPROJECT_API AChaseStatue : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChaseStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* CSCol; //콜리전

public:
	//상태 변수
	UPROPERTY()
	ChaseStatueState mState = ChaseStatueState::Move;

	//상태 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void TickPatrol(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = State)
	void TickMove(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = State)
	void TickAttack(const float& DeltaTime); //플레이어 공격 -> 사망이벤트 발생

public:
	UPROPERTY()
	class ATestCharacter* Target; //플레이어 추격

	UPROPERTY()
	class AChaseStatue* me; // 나 자신

	UPROPERTY()
	class AAIController* ChaseAI; //네비게이션 길찾기

	void SetState(ChaseStatueState newState);

	//길찾기 수행시 랜덤 위치
	FVector randomPos;
	//랜덤 위치 가져오기
	bool GetRandomPositionNavMesh(FVector CenterLocation, float radius, FVector& dest);

	FVector PatrolPoint;

public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =6;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State = 1;
	
};

