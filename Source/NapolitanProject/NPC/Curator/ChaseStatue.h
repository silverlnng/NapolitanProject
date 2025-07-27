// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/Interact/Souvenir/SouvenirActor.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
#include "ChaseStatue.generated.h"

UENUM(BlueprintType)
enum class ChaseStatueState : uint8 {
	Idle,
	Move,
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
	ChaseStatueState mState;

	//상태 함수
	UFUNCTION(BlueprintCallable, Category = State)
	void TickIdle(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = State)
	void TickMove(const float& DeltaTime);

public:
	UPROPERTY()
	class AChaseStatue* me; // 큐레이터 자신

	UPROPERTY()
	class AAIController* ChaseAI; //네비게이션 길찾기

	//길찾기 수행시 랜덤 위치
	FVector randomPos;
	//랜덤 위치 가져오기
	bool GetRandomPositionNavMesh(FVector CenterLocation, float radius, FVector& dest);

	FVector PatrolPoint;

	UFUNCTION()
	void CuratorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =6;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State = 1;

	int32 ItemNumber = 0; //노인이 준 

	bool bClear;

public:
	virtual void ResultEvent(int32 result) override;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void ChangeCleared() override;

public:
	UPROPERTY()
	class UNPC_CuratorAnim* CuratorAnim;

	void SetState(ChaseStatueState newstate);
	
public:
	//유품을 스폰하는 함수
	void SpawnItems();

	bool bItemSpawned;

	UPROPERTY()
	class ASouvenirActor* SouvenirActor;

	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenirActor> SouvenirClass;

	UPROPERTY()
	class ASunFloorDoorToLobby* DoorToLobby;
	
	UPROPERTY()
	class ALightControlActor* LightControlActor;

	void LightEffect();

public:
	////////// 사망 관련 부분 ////////////////
	bool bIsDeadEnding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* MonsterCamera; //점프스케어 카메라 - 사망 시 사용 

	//사망 함수
	UFUNCTION()
	void CuratorDead();

	UFUNCTION()
	void SwitchToMonsterCamera();

	void SetActorViewTarget(UCameraComponent* TargetCamera);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass; //카메라 쉐이크

	//사망용 스크립트
	UPROPERTY(EditAnywhere)
	FString description=FString(TEXT("큐레이터에게 살해당했다"));
	
};

