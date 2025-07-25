// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NapolitanProject/Interact/ItemActor.h"
#include "NapolitanProject/Interact/SouvenirActor.h"
#include "NapolitanProject/NPC/NPCCharacter.h"
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
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	UPROPERTY(EditDefaultsOnly,Category=Item)
	class AItemActor* ItemHead;
	//머리
	UPROPERTY(EditDefaultsOnly,Category=Item)
	class UStaticMeshComponent* HeadStaticMesh;

	UMaterialInstanceDynamic* DynamicMaterial4;  // 동적 머터리얼 인스턴스
	UMaterialInstanceDynamic* DynamicMaterial5;  // 동적 머터리얼 인스턴스

	//스테틱 메시에 사용할 디졸브 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial4; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	UMaterialInterface* DissolveMaterial5;  
	
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
	float IdleDelayTime = 1.5f; //대기 시간

	float CleaningTime = 0;
	float RandomCleaningTime;
	bool bCleaning;

	FVector LastVisitedPoint; //마지막으로 방문한 점 위치 저장
	FVector TargetPoint; //타겟 포인트 지점

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

	//////////////////소리///////////////////////////
	
	UPROPERTY(EditAnywhere, Category = "Spawn Items")
	TSubclassOf<ASouvenirActor> SouvenirClass;

	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Point")
	TSubclassOf<AActor> PointClass;

	UPROPERTY()
	TArray<AActor*> BP_PointsArray;

	int32 randomIndex;

public:
	UPROPERTY(EditAnywhere, Category = "MobPoint")
	TSubclassOf<AActor> MobPointClass; //아이템 손에 달기

	UPROPERTY()
	class AActor* MobPointActor;

public:
	//데칼 생성
	void SpawnFootsDecal();

	// 발자국 데칼에 사용할 마테리얼 (에디터에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	UMaterialInterface* FootstepDecalMaterial;

	// 데칼의 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	FVector DecalSize = FVector(200.f, 220.f, 220.f); // X: 길이, Y: 너비, Z: 높이

	// 데칼 지속 시간 (사라지기 전까지 유지되는 시간)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	float DecalLifetime = 9.f;

	bool bIsResult = false;
	
	UPROPERTY(EditAnywhere)
	TArray<FVector> points = {
		FVector(-2188.554545f, -3640.667534f, 47.877220f),
		FVector(-1638.554545f, -3640.667534f, 47.877220f),
		FVector(-1218.554545f, -3640.667534f, 47.877220f),
		FVector(-1148.554545f, -3310.667534f, 47.877220f),
		FVector(-1148.554545f, -2970.667534f, 47.877220f),
		FVector(-1208.554545f, -2590.667534f, 47.877220f),
		FVector(-1468.554545f, -2470.667534f, 47.877220f),
		FVector(-1928.554545f, -2430.667534f, 47.877220f),
		FVector(-2408.554545f, -2380.667534f, 47.877220f),
		FVector(-1648.554545f, -2930.667534f, 47.877220f),
		FVector(-1478.554545f, -3310.667534f, 47.877220f)
	};
};