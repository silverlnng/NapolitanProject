// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCCharacter.h"
#include "GameFramework/Character.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NPC_Cleaner.generated.h"

UENUM(BlueprintType)
enum class CleanerState : uint8
{
	Idle,
	Cleaning,
	Move,
	Stop,
};


UCLASS()
class NAPOLITANPROJECT_API ANPC_Cleaner : public ANPCCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Cleaner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ResultEvent(int32 result) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 NPC_ID =5; //청소부의 번호

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 State =1;
	
	virtual void Interact() override;

	virtual int32 GetNPCID() override;

	virtual int32 GetState() override;

	virtual void ChangeCleared() override; //사라지고 난 뒤 클리어

public:
	//상태 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = State)
	CleanerState mState = CleanerState::Idle; //우선 기본은 대기 상태

	//상태함수
	UFUNCTION(BlueprintCallable, Category = State)
	void TickIdle(const float& DeltaTime); //일시 대기, 만약 플레이어 상호작용시 대기 상태로 전환

	UFUNCTION(BlueprintCallable, Category = State)
	void TickMove(const float& DeltaTime); //움직임

	UFUNCTION(BlueprintCallable, Category = State)
	void TickCleaning(const float& DeltaTime); //청소 애니메이션 작동

	UFUNCTION(BlueprintCallable, Category = State)
	void TickStop(const float& DeltaTime); //플레이어와 대화시 완전 멈춤

	float CurrentTime = 0; //현재 시간
	float IdleDelayTime = 3.f; //대기 시간

	FVector LastVisitedPoint; //마지막으로 방문한 점 위치 저장

	void SetState(CleanerState newState);

	UPROPERTY()
	class UNPCCleanerAnim* CleanerAnim;

	UPROPERTY()
	class AAIController* AI;

	FTimerHandle TimerHandle;

	//디졸브
	//사라지는 효과
	bool bisDissolve = false;
	float dissolveAnimValue;
	
	UMaterialInstanceDynamic* DynamicMaterial1;  // 몸 머터리얼
	UMaterialInstanceDynamic* DynamicMaterial2;  // 목 머터리얼
	UMaterialInstanceDynamic* DynamicMaterial3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial1;  // 블루프린트에서 지정할 머터리얼, 1-0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial2;  // 1-1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial3;

	//아이템, 유품을 스폰하는 함수
	void SpawnItems();
	bool bItemSpawned = false;

	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenirActor> SouvenirClass;

	////머리
	UPROPERTY(EditDefaultsOnly,Category=Item)
	class UStaticMeshComponent* HeadStaticMesh;

	

};
