// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChaseStatue.generated.h"

UENUM(BlueprintType)
enum class ChaseStatueState : uint8 {
	Idle,
	Move,
	Attack,
};

UCLASS()
class NAPOLITANPROJECT_API AChaseStatue : public ACharacter
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
	ChaseStatueState mState = ChaseStatueState::Idle;

	//상태 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void TickIdle(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = State)
	void TickMove(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = State)
	void TickAttack(const float& DeltaTime); //플레이어 공격 -> 사망이벤트 발생

public:
	UPROPERTY()
	class ATestCharacter* Target; //플레이어 추격
};
